#include "object.h"

Object::Object(const IpfsHash &hash)
    : hash_(hash)
{
}

Object::Object(QString hash)
    : hash_(IpfsHash(hash))
{
}

const IpfsHash &Object::hash() const
{
    return hash_;
}
