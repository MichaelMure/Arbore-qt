#ifndef IPFSGET_H
#define IPFSGET_H

#include "ipfs/abstractipfscommand.h"
#include "ipfs/ipfshash.h"
#include <QObject>

class IpfsGet : public AbstractIpfsCommand
{
    Q_OBJECT
public:
    explicit IpfsGet(QObject *parent = 0);
    virtual ~IpfsGet() {}

    /* Will not work, https://github.com/ipfs/go-ipfs/issues/1210 */
    void write_on_disk(const IpfsHash &hash, const QString &path);

    // AbstractIpfsCommand interface
public:
    void init();
    void on_reply(const QJsonObject *json);
};

#endif // IPFSGET_H
