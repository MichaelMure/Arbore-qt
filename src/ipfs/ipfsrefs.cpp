#include "ipfsrefs.h"

#include "ipfs.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QTextStream>

const QString API_COMMAND = "refs";

IpfsRefs::IpfsRefs(QObject *parent)
    : QObject(parent)
{
}

RefsReply *IpfsRefs::recursive_refs(const IpfsHash &hash)
{
    QUrl url = Ipfs::instance().api_url(API_COMMAND);
    QUrlQuery query;

    query.addQueryItem("arg", hash.ToString());
    query.addQueryItem("r", "true");

    // doesn't work as expected for now
    //query.addQueryItem("u", "true");

    url.setQuery(query);
    QNetworkReply *network_reply = Ipfs::instance().manual_query(url);

    RefsReply *refs_reply = new RefsReply();

    connect(network_reply, &QNetworkReply::finished,
            refs_reply, [network_reply, refs_reply]()
    {
        if(network_reply->error())
        {
            qDebug() << "http error: " << network_reply->errorString() << endl;
            qDebug() << "request: " << network_reply->request().url() << endl;
            return;
        }

        QString str = network_reply->readAll();
        qDebug() << "http reply : " << str;

        QTextStream stream(network_reply->readAll());

        while(!stream.atEnd())
        {
            refs_reply->refs << new IpfsHash(stream.readLine());
        }

        emit refs_reply->finished();

        network_reply->deleteLater();
    });

    return refs_reply;
}


RefsReply::~RefsReply()
{
    int nb = refs.count();
    for(int i = 0; i < nb; i++)
    {
        delete refs[i];
    }
}
