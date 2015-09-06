#include "object.h"
#include "objectcache.h"

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
