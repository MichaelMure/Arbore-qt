#include "file.h"

#include "ipfs/ipfs.h"
#include "ipfs/ipfsrefs.h"

#include <QDebug>

struct Block
{
    IpfsHash hash;
    uint size;
};

File::File(const IpfsHash &hash, const QString &name)
    : Object(hash, name), metadata_local_(false)
{
    connect(&(Ipfs::instance()->refs), SIGNAL(objectAdded(IpfsHash)),
            this, SLOT(objectAdded(IpfsHash)));
    connect(&(Ipfs::instance()->refs), SIGNAL(objectRemoved(IpfsHash)),
            this, SLOT(objectRemoved(IpfsHash)));

    RefsReply *reply = Ipfs::instance()->refs.recursive_refs(hash);

    connect(reply, &RefsReply::finished, [reply, hash, this]()
    {
        qDebug() << hash.ToString() << "  " << reply->refs.size();

        foreach (const IpfsHash* block_hash, reply->refs)
        {
            Block *block = new Block();

            block->hash = *block_hash;
            block->size = 1000; // TODO

            this->blocks_[block->hash] = block;
        }

        metadata_local_ = true;

        emit localityChanged();
    });
}

File::File(const QString &hash, const QString &name)
    : File(IpfsHash(hash), name)
{
}

File::~File()
{
    for(QHash<IpfsHash, Block*>::iterator i = blocks_.begin(); i != blocks_.end(); i++)
    {
        delete i.value();
    }
}

Object::ObjectType File::type() const
{
    return ObjectType::FILE;
}

uint File::size_total() const
{
    uint size_total = 0;
    for(QHash<IpfsHash, Block*>::const_iterator i = blocks_.constBegin(); i != blocks_.constEnd(); i++)
    {
        size_total += i.value()->size;
    }
    return size_total;
}

uint File::size_local() const
{
    uint size_local = 0;
    for(QHash<IpfsHash, Block*>::const_iterator i = blocks_.constBegin(); i != blocks_.constEnd(); i++)
    {
        if(Ipfs::instance()->refs.is_object_local(i.key()))
        {
            size_local += i.value()->size;
        }
    }
    return size_local;
}

uint File::block_total() const
{
    return blocks_.size() + 1;
}

uint File::block_local() const
{
    uint block_local = 0;

    if(Ipfs::instance()->refs.is_object_local(hash_))
    {
        block_local++;
    }

    for(QHash<IpfsHash, Block*>::const_iterator i = blocks_.constBegin(); i != blocks_.constEnd(); i++)
    {
        if(Ipfs::instance()->refs.is_object_local(i.key()))
        {
            block_local++;
        }
    }
    return block_local;
}

uint File::file_total() const
{
    return 1;
}

uint File::file_local() const
{
    return (block_total() == block_local());
}

bool File::metadata_local() const
{
    return metadata_local_;
}

void File::objectAdded(const IpfsHash &hash)
{
    if(blocks_.contains(hash))
    {
        emit localityChanged();
    }
}

void File::objectRemoved(const IpfsHash &hash)
{
    if(blocks_.contains(hash))
    {
        emit localityChanged();
    }
}
