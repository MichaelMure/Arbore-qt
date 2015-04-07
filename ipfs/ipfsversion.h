#ifndef IPFS_VERSION_H
#define IPFS_VERSION_H

#include "ipfs/ipfscommand.h"
#include <QObject>

class IpfsVersion : IpfsCommand
{
    Q_OBJECT
public:
    explicit IpfsVersion(QObject *parent = 0);

    QString ToString() const;

    int major() const;
    int minor() const;
    int micro() const;

    void init();
    void on_reply(const QJsonObject *json);

private:
    int major_;
    int minor_;
    int micro_;

};

#endif // IPFS_VERSION_H
