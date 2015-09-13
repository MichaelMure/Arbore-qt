#ifndef IPFSPIN_H
#define IPFSPIN_H

#include <QObject>
#include "ipfs/ipfshash.h"

class IpfsPin : public QObject
{
    Q_OBJECT

public:
    enum IpfsPinType
    {
        DIRECT,
        RECURSIVE,
        INDIRECT,
        ALL
    };

public:
    explicit IpfsPin(QObject *parent = 0);
    virtual ~IpfsPin() {}

    void add_pin(const IpfsHash &hash, bool recursive = false);
    void ls_pin(IpfsPinType pin_type = DIRECT);
    void rm_pin(const IpfsHash &hash, bool recursive = false);
};

#endif // IPFSPIN_H

