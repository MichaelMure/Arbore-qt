#ifndef IPFSID_H
#define IPFSID_H

#include <QObject>
#include "ipfs/iapilistener.h"
#include "ipfs/ipfshash.h"


class IpfsId : public QObject, public IApiListener
{
    Q_OBJECT
public:
    explicit IpfsId(QObject *parent = 0);
    virtual ~IpfsId() {}

    const IpfsHash& id() const;

public:
    void init();
    void on_reply(const QJsonObject *json);
    bool valid_data() const;

private:
    bool valid_data_;
    IpfsHash *id_;
};

#endif // IPFSID_H
