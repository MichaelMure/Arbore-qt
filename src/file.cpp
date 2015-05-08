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
            block->size = 0; // TODO

            this->blocks_[block->hash] = block;
        }
    });
}

File::~File()
{
    for(QHash<IpfsHash, Block*>::iterator i = blocks_.begin(); i != blocks_.end(); i++)
    {
        delete i.value();
    }
}
