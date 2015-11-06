#include "file.h"

#include "ipfs/ipfs.h"
#include "ipfs/ipfsobject.h"

#include <QDebug>

struct Block
{
    IpfsHash hash;
    uint size;
};

File::File(const IpfsHash &hash, const QString &name)
    : Object(hash, name),
      metadata_local_(false),
      root_block_size_(0)
{
    // listen to ipfs refs signal to update the locality of blocks
    connect(Ipfs::instance(), SIGNAL(objectAdded(IpfsHash)),
            this, SLOT(objectAdded(IpfsHash)));
    connect(Ipfs::instance(), SIGNAL(objectRemoved(IpfsHash)),
            this, SLOT(objectRemoved(IpfsHash)));

    FileReply *flreply = Ipfs::instance()->file.ls(hash);
    connect(flreply, &FileReply::finished, [flreply, this]()
    {
        this->root_block_size_ = flreply->size;

        delete flreply;
    });

    ObjectLinkReply *olreply = Ipfs::instance()->object.links(hash);
    connect(olreply, &ObjectLinkReply::finished, [olreply, hash, this]()
    {
        foreach (const ObjectLinkEntry* entry, olreply->entries) {
            Block *block = new Block();

            block->hash = entry->hash();
            block->size = entry->size();

            this->blocks_[block->hash] = block;
        }

        metadata_local_ = true;

        emit localityChanged();

        delete olreply;
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
    uint size_total = root_block_size_;
    for(QHash<IpfsHash, Block*>::const_iterator i = blocks_.constBegin(); i != blocks_.constEnd(); i++)
    {
        size_total += i.value()->size;
    }
    return size_total;
}

uint File::size_local() const
{
    uint size_local = root_block_size_;
    for(QHash<IpfsHash, Block*>::const_iterator i = blocks_.constBegin(); i != blocks_.constEnd(); i++)
    {
        if(Ipfs::instance()->is_object_local(i.key()))
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

    if(Ipfs::instance()->is_object_local(hash_))
    {
        block_local++;
    }

    for(QHash<IpfsHash, Block*>::const_iterator i = blocks_.constBegin(); i != blocks_.constEnd(); i++)
    {
        if(Ipfs::instance()->is_object_local(i.key()))
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
