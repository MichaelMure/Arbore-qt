#include "share.h"

#include "object.h"
#include "directory.h"
#include "file.h"
#include "objectiterator.h"

#include <cassert>
#include <QDateTime>
#include <QDebug>
#include <QStringBuilder>

Share::Share(QObject *parent):
    QObject(parent),
    id_(-1),
    creation_date_(QDateTime::currentDateTime()),
    starred_(false),
    state_(UNITIALIZED)
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
    qDebug() << "STARRED " << starred;
}

QString Share::textual_arborescence() const
{
    QString result;
    for(ObjectIterator it = ObjectIterator(this->objects_[0]); it != ObjectIterator(); it++)
    {
        result += QString("--").repeated(it.depth());
        result += (*it).name();
        result += "\n";
    }

    return result;
}

ShareState Share::state() const
{
    return state_;
}

float Share::progress() const
{
    uint total = block_total();
    qDebug() << "progress " << block_local() << "/" << total;

    if(total == 0)
        return 0.0;

    return (float)block_local() / (float)total;
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

void Share::add_hash(const IpfsHash &hash, Object::ObjectType type)
{
    Object *obj;
    switch (type)
    {
    case Object::ObjectType::DIRECTORY:
        obj = new Directory(hash);
        break;
    case Object::ObjectType::FILE:
        obj = new File(hash);
    default:
        qDebug() << "Unknow object type retrieved from database.";
        assert(false);
        break;
    }

    objects_ << obj;

    connect(obj, SIGNAL(localityChanged()),
            this, SIGNAL(dataChanged()));
}

void Share::set_title(const QString &title)
{
    title_ = title;
    emit shareChanged();
}

void Share::set_description(const QString &description)
{
    description_ = description;
    emit shareChanged();
}

void Share::set_path(const QDir &path)
{
    path_ = path;
    emit shareChanged();
}

void Share::set_date_creation(const QDateTime &date)
{
    creation_date_ = date;
    emit shareChanged();
}
