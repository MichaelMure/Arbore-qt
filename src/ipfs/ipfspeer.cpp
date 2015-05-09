#include "ipfs/ipfspeer.h"
#include "ipfs/ipfs.h"

#include <QStringList>

IpfsPeer::IpfsPeer(QString multiaddr)
    : multiaddr_(multiaddr),
      hash_(multiaddr.split("/").last())
{
}

IpfsPeer::IpfsPeer(const IpfsPeer &other)
    : multiaddr_(other.multiaddr_),
      hash_(other.hash_)
{
}

QString IpfsPeer::ToString() const
{
    return multiaddr_;
}

QString IpfsPeer::multiaddr() const
{
    return multiaddr_;
}

const IpfsHash& IpfsPeer::hash() const
{
    return hash_;
}
