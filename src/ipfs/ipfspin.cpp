#include "ipfspin.h"
#include "ipfs/ipfs.h"

#include <QUrl>
#include <QUrlQuery>

const QString API_COMMAND = "pin";

IpfsPin::IpfsPin(QObject *parent)
    : QObject(parent)
{
}

void IpfsPin::add_pin(const IpfsHash &hash, bool recursive)
{
    QUrl url = Ipfs::instance().api_url(API_COMMAND + "/add");
    QUrlQuery query;

    query.addQueryItem("arg", hash.ToString());

    if(recursive)
        query.addQueryItem("r", "true");

    url.setQuery(query);

    Ipfs::instance().query(url);
}

void IpfsPin::ls_pin(IpfsPinType pin_type)
{
    QUrl url = Ipfs::instance().api_url(API_COMMAND + "/ls");
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
    Ipfs::instance().query(url);
}
