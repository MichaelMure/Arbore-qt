#include "ipfsget.h"
#include "ipfs/ipfs.h"

#include <QUrl>
#include <QUrlQuery>

const QString API_COMMAND = "get";

IpfsGet::IpfsGet(QObject *parent)
    : QObject(parent)
{
}

/*void IpfsGet::write_on_disk(const IpfsHash &hash, const QString &path)
{
    QUrl url = Ipfs::instance().api_url(API_COMMAND);
    QUrlQuery query;

    query.addQueryItem("arg", hash.ToString());
    query.addQueryItem("o", path);

    url.setQuery(query);
    Ipfs::instance().query(url);
}*/
