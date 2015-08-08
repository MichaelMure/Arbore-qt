#ifndef IPFSID_H
#define IPFSID_H

#include <QObject>
#include "ipfs/ipfshash.h"


class IpfsId : public QObject
{
    Q_OBJECT
public:
    explicit IpfsId(QObject *parent = 0);
    virtual ~IpfsId() {}

    const IpfsHash& id() const;

public:
    void init();
    bool valid_data() const;

private:
    bool valid_data_;
    IpfsHash *id_;
};

#endif // IPFSID_H
