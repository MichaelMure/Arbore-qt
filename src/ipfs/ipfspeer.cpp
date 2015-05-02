#include "ipfs/ipfspeer.h"
#include "ipfs/ipfs.h"

#include <QStringList>

IpfsPeer::IpfsPeer(QString full_desc)
    : full_desc_(full_desc),
      hash_(full_desc.split("/").last())
{
}

QString IpfsPeer::ToString() const
{
    return full_desc_;
}

QString IpfsPeer::full_desc() const
{
    return full_desc_;
}

const IpfsHash& IpfsPeer::hash() const
{
    return hash_;
}
