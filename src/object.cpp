#include "object.h"
#include "objectcache.h"
#include "directory.h"
#include "file.h"

#include "ipfs/ipfs.h"
#include "ipfs/ipfsfile.h"

Object::ObjectType Object::decode_type(const QString &str)
{
    if(str == "File")
    {
        return FILE;
    }

    if(str == "Directory")
    {
        return DIRECTORY;
    }

    return INVALID;
}

ObjectReply *Object::from_hash(const IpfsHash &hash)
{
    ObjectReply *result = new ObjectReply();

    FileReply *reply = Ipfs::instance()->file.ls(hash);

    connect(reply, FileReply::finished, [reply, result]()
    {
        switch (reply->type) {
        case ObjectType::DIRECTORY:
            result->object = new Directory(reply);
            break;
        case ObjectType::FILE:
            result->object = new File(reply->hash);
            break;
        case ObjectType::RAW:
        case ObjectType::METADATA:
        default:
            Q_ASSERT(false);
        }

        emit result->finished();
        delete reply;
    });

    return result;
}

Object::Object(const IpfsHash &hash, const QString &name)
    : name_(name), hash_(hash)
{
    // Automatically insert the object in the cache
    ObjectCache::instance()->insert(this);
}

Object::Object(const QString &hash, const QString &name)
    : Object(IpfsHash(hash), name)
{
}

const QString &Object::name() const
{
    return name_;
}

const IpfsHash &Object::hash() const
{
    return hash_;
}
