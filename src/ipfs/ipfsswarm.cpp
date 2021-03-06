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
    QUrl url = Ipfs::instance()->api_url(API_COMMAND + "/peers");
    IpfsAccess *access = Ipfs::instance()->query(url);

    connect(access, &IpfsAccess::finished,
            this, [this, access]()
    {
        const QJsonObject &json = access->json();

        this->peers_.clear();
        foreach (const QJsonValue &value, json.value("Strings").toArray())
        {
            try
            {
                // weird reply can happen
                // See https://github.com/ipfs/go-ipfs/issues/1560
                this->peers_ << IpfsPeer(value.toString());
            }
            catch(std::exception& e)
            {
                qDebug() << e.what() << value.toString();
            }

        }
        this->valid_data_ = true;

        delete access;
    });
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
    if(Ipfs::instance()->online())
        refresh_peers();
}
