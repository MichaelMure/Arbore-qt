#include "file.h"

#include "ipfs/ipfs.h"
#include "ipfs/ipfsrefs.h"

struct Block
{
    IpfsHash hash;
    uint size;
};

File::File(const IpfsHash &hash)
    : Object(hash)
{
    RefsReply *reply = Ipfs::instance().refs.recursive_refs(hash);

    connect(reply, &RefsReply::finished, [reply, this]()
    {
        foreach (const IpfsHash* block_hash, reply->refs)
        {
            Block *block = new Block();

            block->hash = *block_hash;
            block->size = 1000; // TODO

            this->blocks_[block->hash] = block;
        }
    });
}

File::File(const QString &hash)
    : File(IpfsHash(hash))
{
}

File::~File()
{
    for(QHash<IpfsHash, Block*>::iterator i = blocks_.begin(); i != blocks_.end(); i++)
    {
        delete i.value();
    }
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
        if(Ipfs::instance().refs.is_object_local(i.key()))
        {
            size_local += i.value()->size;
        }
    }
    return size_local;
}

uint File::block_total() const
{
    return blocks_.size();
}

uint File::block_local() const
{
    uint block_local = 0;
    for(QHash<IpfsHash, Block*>::const_iterator i = blocks_.constBegin(); i != blocks_.constEnd(); i++)
    {
        if(Ipfs::instance().refs.is_object_local(i.key()))
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
