#include "objectcache.h"
#include "object.h"

Q_GLOBAL_STATIC(ObjectCache, singleton)

ObjectCache::ObjectCache()
{
}

ObjectCache::~ObjectCache()
{
}

ObjectCache *ObjectCache::instance()
{
    return singleton();
}

bool ObjectCache::contains(const IpfsHash &hash) const
{
    return objects_.contains(hash);
}

Object *ObjectCache::get(const IpfsHash &hash) const
{
    if(objects_.contains(hash))
    {
        return objects_[hash];
    }

    return NULL;
}

void ObjectCache::insert(Object *obj)
{
    Q_ASSERT(!objects_.contains(obj->hash()));

    objects_[obj->hash()] = obj;
}
