#include "directory.h"

#include "ipfs/ipfs.h"
#include "ipfs/ipfsls.h"

struct Child
{
    IpfsHash hash;
    QString name;
    uint size;
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

            this->child_hashes_[child->hash] = child;
            this->child_names_[child->name] = child;
        }
    });
}

Directory::~Directory()
{
    for(QHash<IpfsHash, Child*>::iterator i = child_hashes_.begin(); i != child_hashes_.end(); i++)
    {
        delete i.value();
    }
}

