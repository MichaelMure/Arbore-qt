#ifndef IPFSADD_H
#define IPFSADD_H

#include <QObject>
#include "ipfs/ipfshash.h"

class AddReply;

class IpfsAdd : public QObject
{
    Q_OBJECT
public:
    explicit IpfsAdd(QObject *parent = 0);
    virtual ~IpfsAdd() {}

    AddReply *add(QString filepath);
};

class AddReply : public QObject
{
    Q_OBJECT
signals:
    void finished();

public:
    IpfsHash hash;
};

#endif // IPFSADD_H
