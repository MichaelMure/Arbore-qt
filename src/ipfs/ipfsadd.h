#ifndef IPFSADD_H
#define IPFSADD_H

#include <QObject>
#include "ipfs/ipfshash.h"
#include "ipfs/hashreply.h"

class AddReply;

class IpfsAdd : public QObject
{
    Q_OBJECT
public:
    explicit IpfsAdd(QObject *parent = 0);
    virtual ~IpfsAdd() {}

    HashReply* add(QString filepath);
};

#endif // IPFSADD_H
