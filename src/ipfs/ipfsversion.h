#ifndef IPFS_VERSION_H
#define IPFS_VERSION_H

#include "ipfs/abstractipfscommand.h"
#include <QObject>

class IpfsVersion : AbstractIpfsCommand
{
    Q_OBJECT
public:
    explicit IpfsVersion(QObject *parent = 0);
    virtual ~IpfsVersion() {}
    void init();
    void on_reply(const QJsonObject *json);

    QString ToString() const;

    int major() const;
    int minor() const;
    int micro() const;

private:
    int major_;
    int minor_;
    int micro_;

};

#endif // IPFS_VERSION_H
