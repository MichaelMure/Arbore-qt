#include "ipfspin.h"
#include "ipfs/ipfs.h"

#include <QUrl>
#include <QUrlQuery>

const QString API_URL = "http://127.0.0.1:5001/api/v0/pin/";

IpfsPin::IpfsPin(QObject *parent)
    : AbstractIpfsCommand(parent)
{
}

void IpfsPin::init() { }

void IpfsPin::add_pin(IpfsHash &hash, bool recursive)
{
    QUrl url = API_URL + "add";
    QUrlQuery query;

    query.addQueryItem("arg", hash.ToString());

    if(recursive)
        query.addQueryItem("r", "true");

    url.setQuery(query);
    Ipfs::instance().query(url, this);
}

void IpfsPin::ls_pin(IpfsPinType pin_type)
{
    QUrl url = API_URL + "ls";
    QUrlQuery query;

    QString type;
    switch(pin_type)
    {
    case DIRECT:
        type = "direct"; break;
    case RECURSIVE:
        type = "recursive"; break;
    case INDIRECT:
        type = "indirect"; break;
    case ALL:
        type = "all"; break;
    }

    query.addQueryItem("type", type);

    url.setQuery(query);
    Ipfs::instance().query(url, this);
}

void IpfsPin::on_reply(const QJsonObject *json)
{
    Q_UNUSED(json);
}
