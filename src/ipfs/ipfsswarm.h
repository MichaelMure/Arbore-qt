#ifndef IPFSSWARM_H
#define IPFSSWARM_H

#include <QObject>
#include "ipfs/iapilistener.h"
#include "ipfs/ipfspeer.h"

class IpfsSwarm : public QObject, public IApiListener
{
    Q_OBJECT
public:
    explicit IpfsSwarm(QObject *parent = 0);
    virtual ~IpfsSwarm() {}

    const QList<IpfsPeer> &peers() const;
    uint peer_count() const;

public:
    void init();
    void on_reply(const QJsonObject *json);
    bool valid_data() const;

private:
    void refresh_peers();

private:
    bool valid_data_;
    QList<IpfsPeer> peers_;

protected:
    void timerEvent(QTimerEvent *);
};

#endif // IPFSSWARM_H
