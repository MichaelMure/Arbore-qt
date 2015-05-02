#ifndef IPFSID_H
#define IPFSID_H

#include "ipfs/abstractipfscommand.h"
#include "ipfs/ipfshash.h"
#include <QObject>


class IpfsId : public AbstractIpfsCommand
{
    Q_OBJECT
public:
    explicit IpfsId(QObject *parent = 0);
    virtual ~IpfsId() {}

    const IpfsHash& id() const;

    // AbstractIpfsCommand interface
public:
    void init();
    void on_reply(const QJsonObject *json);
    bool valid_data() const;

private:
    bool valid_data_;
    IpfsHash *id_;
};

#endif // IPFSID_H
