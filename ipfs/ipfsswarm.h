#ifndef IPFSSWARM_H
#define IPFSSWARM_H

#include "ipfs/ipfscommand.h"
#include "ipfs/ipfspeer.h"
#include <QObject>

class IpfsSwarm : public IpfsCommand
{
    Q_OBJECT
public:
    explicit IpfsSwarm(QObject *parent = 0);
    void init();
    void on_reply(const QJsonObject *json);

    const QList<IpfsPeer> &peers() const;

private:
    void refresh_peers();

    QList<IpfsPeer> peers_;
};

#endif // IPFSSWARM_H
