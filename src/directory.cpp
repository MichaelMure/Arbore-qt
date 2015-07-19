#include "directory.h"

#include "ipfs/ipfs.h"
#include "ipfs/ipfsls.h"
#include "file.h"

#include <QDebug>

struct Child
{
    IpfsHash hash;
    QString name;
    uint size;
    Object *object;
};

Directory::Directory(const IpfsHash &hash)
    : Object(hash)
{
    LsReply *reply = Ipfs::instance().ls.ls(hash);

    connect(reply, &LsReply::finished, [reply, this]()
    {
        foreach (const LsEntry* entry, reply->entries)
        {
            Child *child = new Child();

            child->hash = IpfsHash(entry->hash());
            child->name = entry->name();
            child->size = entry->size();

            switch(entry->type())
            {
            case LsEntry::DIRECTORY:
                child->object = new Directory(entry->hash());
                qDebug() << "found dir " << entry->name();
                break;
            case LsEntry::FILE:
                child->object = new File(entry->hash(), entry->size());
                qDebug() << "found file " << entry->name();
                break;
            case LsEntry::RAW:
            case LsEntry::METADATA:
            default:
                qDebug() << "Error: unsupported object type !";
                continue;
            }

            connect(child->object, SIGNAL(localityChanged()),
                    this, SIGNAL(localityChanged()));

            this->child_hashes_[child->hash] = child;
            this->child_names_[child->name] = child;
        }
    });
}

Directory::Directory(const QString &hash)
    : Directory(IpfsHash(hash))
{
}

Directory::~Directory()
{
    for(QHash<IpfsHash, Child*>::iterator i = child_hashes_.begin(); i != child_hashes_.end(); i++)
    {
        delete i.value();
    }
}

uint Directory::size_total() const
{
    uint size = 0;
    for(QHash<IpfsHash, Child*>::const_iterator i = child_hashes_.constBegin(); i != child_hashes_.constEnd(); i++)
    {
        // Not relying on children file/block size()
        size += i.value()->size;
    }
    return size;
}

uint Directory::size_local() const
{
    uint size_local = 0;
    for(QHash<IpfsHash, Child*>::const_iterator i = child_hashes_.constBegin(); i != child_hashes_.constEnd(); i++)
    {
        size_local += i.value()->object->size_local();
    }
    return size_local;
}

uint Directory::block_total() const
{
    uint block_total = 0;
    for(QHash<IpfsHash, Child*>::const_iterator i = child_hashes_.constBegin(); i != child_hashes_.constEnd(); i++)
    {
        block_total += i.value()->object->block_total();
    }
    return block_total;
}

uint Directory::block_local() const
{
    uint block_local = 0;
    for(QHash<IpfsHash, Child*>::const_iterator i = child_hashes_.constBegin(); i != child_hashes_.constEnd(); i++)
    {
        block_local += i.value()->object->block_local();
    }
    return block_local;
}

uint Directory::file_total() const
{
    uint file_total = 0;
    for(QHash<IpfsHash, Child*>::const_iterator i = child_hashes_.constBegin(); i != child_hashes_.constEnd(); i++)
    {
        file_total += i.value()->object->file_total();
    }
    return file_total;
}

uint Directory::file_local() const
{
    uint file_local = 0;
    for(QHash<IpfsHash, Child*>::const_iterator i = child_hashes_.constBegin(); i != child_hashes_.constEnd(); i++)
    {
        file_local += i.value()->object->file_local();
    }
    return file_local;
}
