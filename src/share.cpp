#include "share.h"

#include "object.h"
#include "directory.h"
#include "file.h"
#include "objectiterator.h"
#include "objectcache.h"
#include "ipfs/ipfs.h"
#include "persist/persist.h"

#include <QDateTime>
#include <QDebug>
#include <QStringBuilder>

/*
 * PlantUML
 *
 * @startuml
 * [*] --> CREATING
 * CREATING --> CREATING : add file/dir
 * CREATING  --> READY : dl metadata done
 * READY --> CREATING : add file/dir
 * CREATING  --> WAITING_FOR_DL : start
 * WAITING_FOR_DL --> DOWNLOAD : dl metadata done
 * READY --> DOWNLOAD : start
 * PAUSED --> DOWNLOAD : start
 * DOWNLOAD --> SHARING : ended
 * DOWNLOAD --> PAUSED : pause
 *
 * [*] --> SHARING : share
 *
 * @enduml
 */

Share::Share(QObject *parent):
    QObject(parent),
    id_(-1),
    creation_date_(QDateTime::currentDateTime()),
    starred_(false),
    state_(CREATING)
{
}

int Share::id() const
{
    return id_;
}

const QString& Share::title() const
{
    return title_;
}

const QString Share::description() const
{
    if(!description_.isEmpty() || objects_.count() == 0)
        return description_;

    // If no description is available, we build a list of the first files
    QString desc;
    for(QList<Object*>::const_iterator i = objects_.constBegin(); i != objects_.constEnd(); i++)
    {
        for(ObjectIterator it = ObjectIterator(*i); it != ObjectIterator(); it++)
        {
            if(!(*it).name().isEmpty())
            {
                desc += (*it).name();
                desc += ", ";
            }
            if(desc.count() > 100)
            {
                desc += "...";
                return desc;
            }
        }
    }

    return desc;
}

const QDir &Share::path() const
{
    return path_;
}

const QDateTime &Share::date_creation() const
{
    return creation_date_;
}

bool Share::starred() const
{
    return starred_;
}

void Share::set_starred(const bool &starred)
{
    starred_ = starred;
    emit shareChanged();
}

QString Share::textual_arborescence() const
{
    if(this->objects_.count() == 0)
        return "";

    QString result;
    for(ObjectIterator it = ObjectIterator(this->objects_[0]); it != ObjectIterator(); it++)
    {
        result += QString("--").repeated(it.depth());
        result += (*it).name();
        result += "\n";
    }

    return result;
}

Share::ShareState Share::state() const
{
    return state_;
}

float Share::progress() const
{
    if(state_ == CREATING)
        return 0.0;

    uint total = size_total();

    if(total == 0)
        return 0.0;

    return (float)size_local() / (float)total;
}

uint Share::size_total() const
{
    uint size_total = 0;
    for(QList<Object*>::const_iterator i = objects_.constBegin();
        i != objects_.constEnd();
        i++)
    {
        size_total += (*i)->size_total();
    }
    return size_total;
}

uint Share::size_local() const
{
    uint size_local = 0;
    for(QList<Object*>::const_iterator i = objects_.constBegin();
        i != objects_.constEnd();
        i++)
    {
        size_local += (*i)->size_local();
    }
    return size_local;
}

uint Share::block_total() const
{
    uint block_total = 0;
    for(QList<Object*>::const_iterator i = objects_.constBegin();
        i != objects_.constEnd();
        i++)
    {
        block_total += (*i)->block_total();
    }
    return block_total;
}

uint Share::block_local() const
{
    uint block_local = 0;
    for(QList<Object*>::const_iterator i = objects_.constBegin();
        i != objects_.constEnd();
        i++)
    {
        block_local += (*i)->block_local();
    }
    return block_local;
}

uint Share::file_total() const
{
    uint file_total = 0;
    for(QList<Object*>::const_iterator i = objects_.constBegin();
        i != objects_.constEnd();
        i++)
    {
        file_total += (*i)->file_total();
    }
    return file_total;
}

uint Share::file_local() const
{
    uint file_local = 0;
    for(QList<Object*>::const_iterator i = objects_.constBegin();
        i != objects_.constEnd();
        i++)
    {
        file_local += (*i)->file_local();
    }
    return file_local;
}

bool Share::metadata_local() const
{
    for(QList<Object*>::const_iterator i = objects_.constBegin();
        i != objects_.constEnd();
        i++)
    {
        if(!(*i)->metadata_local())
        {
            return false;
        }
    }
    return true;
}

void Share::add_hash(const IpfsHash &hash)
{
    Q_ASSERT(state_ == CREATING || state_ == READY);

    Object *obj = ObjectCache::instance()->get(hash);

    if(obj != NULL)
    {
        connect(obj, SIGNAL(localityChanged()),
                this, SLOT(objectChanged()));

        this->objects_ << obj;
        return;
    }

    set_state(CREATING);
    waiting_for_hash_ << hash;
    ObjectReply *reply = Object::from_hash(hash);

    connect(reply, &ObjectReply::finished, [reply, hash, this]()
    {
        connect(reply->object, SIGNAL(localityChanged()),
                this, SLOT(objectChanged()));

        this->objects_ << reply->object;

        waiting_for_hash_.remove(hash);

        if(state_ == WAITING_FOR_DL && waiting_for_hash_.count() == 0)
        {
            trigger_download();
        }

        delete reply;
    });
}

void Share::add_hash(const IpfsHash &hash, Object::ObjectType type)
{
    Q_ASSERT(state_ == CREATING || state_ == READY);

    Object *obj = ObjectCache::instance()->get(hash);

    if(obj != NULL)
    {
        connect(obj, SIGNAL(localityChanged()),
                this, SLOT(objectChanged()));

        this->objects_ << obj;
        return;
    }

    set_state(CREATING);
    waiting_for_hash_ << hash;

    switch (type)
    {
    case Object::ObjectType::DIRECTORY:
        obj = new Directory(hash);
        break;
    case Object::ObjectType::FILE:
        obj = new File(hash);
        break;
    default:
        qDebug() << "Unknow object type retrieved from database.";
        Q_ASSERT(false);
        break;
    }

    objects_ << obj;

    connect(obj, SIGNAL(localityChanged()),
            this, SLOT(objectChanged()));
}

void Share::start()
{
    switch (state_)
    {
    case CREATING:
        set_state(WAITING_FOR_DL);
        break;
    case READY:
    case PAUSED:
        trigger_download();
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void Share::pause()
{
    Q_ASSERT(state_ == DOWNLOADING);
    // Todo
}

void Share::remove()
{
    if(state_ == DOWNLOADING)
    {
        pause();
    }

    for(QList<Object*>::const_iterator i = objects_.constBegin();
        i != objects_.constEnd();
        i++)
    {
        Ipfs::instance()->pin.rm_pin((*i)->hash(), true);
    }

    Ipfs::instance()->repo.gc();
}

void Share::set_title(const QString &title)
{
    title_ = title;
    emit shareChanged();
}

void Share::objectChanged()
{
    if(state_ == DOWNLOADING && block_local() == block_total())
    {
        set_state(SHARING);
    }

    if(metadata_local())
    {
        if(state_ == CREATING)
        {
            set_state(READY);
        }

        if(state_ == WAITING_FOR_DL)
        {
            trigger_download();
        }
    }

    emit dataChanged();
    emit shareChanged();
}

void Share::trigger_download()
{
    for(QList<Object*>::const_iterator i = objects_.constBegin();
        i != objects_.constEnd();
        i++)
    {
        Ipfs::instance()->pin.add_pin((*i)->hash(), true);
    }

    set_state(DOWNLOADING);
}

void Share::set_state(Share::ShareState state)
{
    this->state_ = state;
    Persist::instance()->share.persist(this);
}
