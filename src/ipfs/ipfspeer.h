#ifndef IPFSPEER_H
#define IPFSPEER_H

#include "ipfs/ipfshash.h"

#include <QString>

/**
 * @brief The IpfsPeer class hold an IPFS peers and its related data
 */
class IpfsPeer
{
   // Q_OBJECT
public:
    /**
     * @param full_desc "/ip4/69.181.137.57/tcp/4001/ipfs/QmeUt5MQMYCw5LRx6gvgSzTMfJ93YzrV3E99wsgqUu5P9r"
     */
    IpfsPeer(QString full_desc);
    virtual ~IpfsPeer() {}

    QString ToString() const;

    QString full_desc() const;
    const IpfsHash& hash() const;

private:
    QString full_desc_;
    IpfsHash hash_;
};

#endif // IPFSPEER_H
