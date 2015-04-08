#ifndef IPFSSWARM_H
#define IPFSSWARM_H

#include "ipfs/ipfscommand.h"
#include "ipfs/ipfspeer.h"
#include <QObject>
#include <QTimer>

class IpfsSwarm : public IpfsCommand
{
    Q_OBJECT
public:
    explicit IpfsSwarm(QObject *parent = 0);
    virtual ~IpfsSwarm() {}
    void init();
    void on_reply(const QJsonObject *json);

    const QList<IpfsPeer> &peers() const;

private slots:
    void refresh_peers();

private:
    QList<IpfsPeer> peers_;
    QTimer refreshTimer_;
};

#endif // IPFSSWARM_H
