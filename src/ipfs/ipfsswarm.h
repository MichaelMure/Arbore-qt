#ifndef IPFSSWARM_H
#define IPFSSWARM_H

#include <QObject>
#include "ipfs/iapilistener.h"
#include "ipfs/ipfspeer.h"
#include <QTimer>

class IpfsSwarm : public QObject, public IApiListener
{
    Q_OBJECT
public:
    explicit IpfsSwarm(QObject *parent = 0);
    virtual ~IpfsSwarm() {}

    const QList<IpfsPeer> &peers() const;

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
