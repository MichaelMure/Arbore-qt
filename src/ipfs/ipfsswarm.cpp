#include "ipfsswarm.h"
#include "ipfs/ipfs.h"

#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>

const QString API_COMMAND = "swarm";

IpfsSwarm::IpfsSwarm(QObject *parent)
    : QObject(parent),
      valid_data_(false),
      peers_()
{
    startTimer(10 * 1000); // 10s
}

void IpfsSwarm::init()
{
    refresh_peers();
}

void IpfsSwarm::refresh_peers()
{
    Ipfs::instance().query(API_COMMAND + "/peers", this);

    qDebug() << "refreshing peers";
}

void IpfsSwarm::on_reply(const QJsonObject *json)
{
    peers_.clear();
    foreach (const QJsonValue &value, json->value("Strings").toArray())
    {
        peers_ << IpfsPeer(value.toString());
    }
    valid_data_ = true;
}

bool IpfsSwarm::valid_data() const
{
    return valid_data_;
}

const QList<IpfsPeer> &IpfsSwarm::peers() const
{
    return peers_;
}

uint IpfsSwarm::peer_count() const
{
    return peers_.count();
}

void IpfsSwarm::timerEvent(QTimerEvent *)
{
    refresh_peers();
}
