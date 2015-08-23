#ifndef OBJECTCACHE_H
#define OBJECTCACHE_H

#include <QObject>
#include <QHash>
#include "ipfs/ipfshash.h"

class Object;

class ObjectCache : public QObject
{
    // Only an Object can insert himself in the cache
    friend class Object;

    Q_OBJECT
public:
    ObjectCache();
    virtual ~ObjectCache();

    // Singleton
    static ObjectCache* instance();

private:
    Q_DISABLE_COPY(ObjectCache)

public:
    /**
     * @return true if an Object with the given hash already exist
     */
    bool contains(const IpfsHash &hash) const;

    /**
     * @return an Object from the cache
     */
    Object* get(const IpfsHash &hash) const;

private:
    /**
     * Insert an object in the cache. Automatically called by the object himself
     */
    void insert(Object* obj);

private:
    QHash<IpfsHash, Object*> objects_;
};

#endif // OBJECTCACHE_H
