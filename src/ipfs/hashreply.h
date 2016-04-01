#ifndef HASHREPLY_H
#define HASHREPLY_H

#include <QObject>
#include "ipfs/ipfshash.h"

class HashReply : public QObject
{
    Q_OBJECT
signals:
    void finished();

public:
    IpfsHash hash;
};

#endif // HASHREPLY_H
