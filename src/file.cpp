#include "file.h"

#include "ipfs/ipfs.h"
#include "ipfs/ipfsrefs.h"

#include <QDebug>

struct Block
{
    IpfsHash hash;
    uint size;
};

File::File(const IpfsHash &hash)
    : Object(hash)
{
    RefsReply *reply = Ipfs::instance().refs.recursive_refs(hash);

    qDebug() << "File constructor for: " << hash.ToString();

    connect(reply, &RefsReply::finished, [reply, this]()
    {
        foreach (const IpfsHash* block_hash, reply->refs)
        {
            Block *block = new Block();

            block->hash = *block_hash;
            block->size = 1000; // TODO

            qDebug() << "Found block: " << block_hash->ToString();

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
    uint size = 0;
    for(QHash<IpfsHash, Block*>::const_iterator i = blocks_.constBegin(); i != blocks_.constEnd(); i++)
    {
        size += i.value()->size;
    }
    return size;
}

uint File::size_local() const
{
    // TODO
    return ((double) qrand() / RAND_MAX) * size_total();
}

uint File::block_total() const
{
    return blocks_.size();
}

uint File::block_local() const
{
    // TODO
    return ((double) qrand() / RAND_MAX) * block_total();
}

uint File::file_total() const
{
    return 1;
}

uint File::file_local() const
{
    // TODO
    return ((double) qrand() / RAND_MAX) * file_total();
}
