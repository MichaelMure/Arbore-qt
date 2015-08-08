#include "object.h"

Object::Object(const IpfsHash &hash, const QString &name)
    : name_(name), hash_(hash)
{
}

Object::Object(const QString &hash, const QString &name)
    : name_(name), hash_(IpfsHash(hash))
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
