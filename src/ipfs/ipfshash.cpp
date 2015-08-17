#include "ipfs/ipfshash.h"

IpfsHash::IpfsHash()
    : str_("invalid hash")
{
}

IpfsHash::IpfsHash(QString hash)
    : str_(hash)
{
    // Todo: a better hash validation and exception would be nice
    if(!hash.startsWith("Qm"))
        throw;
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

bool IpfsHash::operator==(const IpfsHash &other) const
{
    return str_ == other.str_;
}

const QString& IpfsHash::ToString() const
{
    return str_;
}
