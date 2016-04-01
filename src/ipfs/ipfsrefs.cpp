#include "ipfsrefs.h"

#include "ipfs.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

const QString API_COMMAND = "refs";

IpfsRefs::IpfsRefs(QObject *parent)
    : QObject(parent)
{
}

RefsReply *IpfsRefs::recursive_refs(const IpfsHash &hash) const
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND);
    QUrlQuery query;

    query.addQueryItem("arg", hash.ToString());
    query.addQueryItem("r", "true");
    query.addQueryItem("u", "true");

    url.setQuery(query);

    IpfsAccess *access = Ipfs::instance()->query(url);
    RefsReply *refs_reply = new RefsReply();

    connect(access, &IpfsAccess::finished,
            this, [access, refs_reply]()
    {
        QString str = access->reply->readAll();

        // Split the concatenated JSON
        // See https://en.wikipedia.org/wiki/JSON_Streaming

        // This might need to be adapted, see https://github.com/ipfs/go-ipfs/issues/1477

        int start = 0;
        int stop = 0;

        while(1)
        {
            start = str.indexOf('{', stop);
            if(start == -1)
                break;

            stop = str.indexOf('}', start);
            if(stop == -1)
                break;

            const QJsonDocument doc = QJsonDocument::fromJson(
                str.midRef(start, stop - start + 1).toUtf8()
            );

            const QJsonObject json = doc.object();

            refs_reply->refs << IpfsHash(json.value("Ref").toString().trimmed());
        }

        emit refs_reply->finished();

        delete access;
    });

    return refs_reply;
}

RefsReply *IpfsRefs::local() const
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND + "/local");
    IpfsAccess *access = Ipfs::instance()->query(url);

    RefsReply *refs_reply = new RefsReply();

    connect(access, &IpfsAccess::finished,
            this, [access, refs_reply]()
    {
            QTextStream stream(access->reply->readAll());

            while(!stream.atEnd())
            {
                refs_reply->refs << IpfsHash(stream.readLine());
            }

            emit refs_reply->finished();

            delete access;
    });

    return refs_reply;
}
