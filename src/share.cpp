#include "share.h"

#include "object.h"
#include "directory.h"

#include <QDebug>

Share::Share(QObject *parent):
    QObject(parent)
{
}

Share::Share(QString name, const IpfsHash &hash, QObject *parent):
    QObject(parent),
    name_(name),
    state_(UNITIALIZED)
{
    Directory *dir = new Directory(hash);
    objects_ << dir;

    connect(dir, SIGNAL(localityChanged()),
            this, SIGNAL(dataChanged()));
}

const QString& Share::name() const
{
    return name_;
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


