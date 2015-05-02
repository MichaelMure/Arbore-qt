#ifndef IPFSSWARM_H
#define IPFSSWARM_H

#include "ipfs/abstractipfscommand.h"
#include "ipfs/ipfspeer.h"
#include <QObject>
#include <QTimer>

class IpfsSwarm : public AbstractIpfsCommand
{
    Q_OBJECT
public:
    explicit IpfsSwarm(QObject *parent = 0);
    virtual ~IpfsSwarm() {}

    const QList<IpfsPeer> &peers() const;

    // AbstractIpfsCommand interface
public:
    void init();
    void on_reply(const QJsonObject *json);
    bool valid_data() const;

private slots:
    void refresh_peers();

private:
    bool valid_data_;
    QList<IpfsPeer> peers_;
    QTimer refreshTimer_;
};

#endif // IPFSSWARM_H
