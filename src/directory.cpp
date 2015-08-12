#include "directory.h"

#include "ipfs/ipfs.h"
#include "ipfs/ipfsls.h"
#include "file.h"

#include <QDebug>

Directory::Directory(const IpfsHash &hash, const QString &name)
    : Object(hash, name)
{
    LsReply *reply = Ipfs::instance().ls.ls(hash);

    connect(reply, &LsReply::finished, [reply, this]()
    {
        foreach (const LsEntry* entry, reply->entries)
        {
            Child *child = new Child();

            child->hash = IpfsHash(entry->hash());
            child->name = entry->name();

            switch(entry->type())
            {
            case ObjectType::DIRECTORY:
                child->object = new Directory(entry->hash(), entry->name());
                qDebug() << "found dir " << entry->name();
                break;
            case ObjectType::FILE:
                child->object = new File(entry->hash(), entry->name());
                qDebug() << "found file " << entry->name();
                break;
            case ObjectType::RAW:
            case ObjectType::METADATA:
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

Directory::Directory(const QString &hash, const QString &name)
    : Directory(IpfsHash(hash), name)
{
}

Directory::~Directory()
{
    for(QHash<IpfsHash, Child*>::iterator i = child_hashes_.begin(); i != child_hashes_.end(); i++)
    {
        delete i.value();
    }
}

Object::ObjectType Directory::type() const
{
    return ObjectType::DIRECTORY;
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

const QHash<QString, Child *> &Directory::getChilds() const
{
    return this->child_names_;
}
