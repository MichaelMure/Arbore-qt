#include "ipfs/ipfshash.h"


IpfsHash::IpfsHash()
    : str_("invalid hash")
{
}

IpfsHash::IpfsHash(QString hash)
    : str_(hash)
{
}

IpfsHash::IpfsHash(const IpfsHash &hash)
    : str_(hash.str_)
{
}

IpfsHash &IpfsHash::operator=(const IpfsHash &other)
{
    str_ = QString(other.str_);
    return *this;
}

const QString& IpfsHash::ToString() const
{
    return str_;
}






