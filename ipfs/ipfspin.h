#ifndef IPFSPIN_H
#define IPFSPIN_H

#include "ipfs/ipfscommand.h"
#include "ipfs/ipfshash.h"
#include <QObject>

enum IpfsPinType { DIRECT, RECURSIVE, INDIRECT, ALL };

class IpfsPin : public IpfsCommand
{
    Q_OBJECT
public:
    explicit IpfsPin(QObject *parent = 0);
    virtual ~IpfsPin() {}
    void init();
    void on_reply(const QJsonObject *json);

    void add_pin(IpfsHash &hash, bool recursive = false);
    void ls_pin(IpfsPinType pin_type = DIRECT);
};

#endif // IPFSPIN_H

