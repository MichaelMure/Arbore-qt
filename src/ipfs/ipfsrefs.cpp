#include "ipfsrefs.h"

#include "ipfs.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QTextStream>

const QString API_COMMAND = "refs";

IpfsRefs::IpfsRefs(QObject *parent)
    : QObject(parent),
      valid_data_(false),
      local_objects_(),
      refreshTimer_(this)
{
    connect(&refreshTimer_, SIGNAL(timeout()),
            this, SLOT(refresh_objects()));

    refreshTimer_.start(10 * 1000); // 10s
}

RefsReply *IpfsRefs::recursive_refs(const IpfsHash &hash) const
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

bool IpfsRefs::is_object_local(const IpfsHash &hash) const
{
    return local_objects_.contains(hash);
}

void IpfsRefs::init()
{
    refresh_objects();
}

bool IpfsRefs::valid_data() const
{
    return valid_data_;
}

void IpfsRefs::refresh_objects()
{
    QUrl url = Ipfs::instance().api_url(API_COMMAND + "/local");
    QNetworkReply *network_reply = Ipfs::instance().manual_query(url);

    connect(network_reply, &QNetworkReply::finished,
            this, [network_reply, this]()
    {
        if(network_reply->error())
        {
            qDebug() << "http error: " << network_reply->errorString() << endl;
            qDebug() << "request: " << network_reply->request().url() << endl;
            return;
        }

        QTextStream stream(network_reply->readAll());

        this->local_objects_.clear();
        while(!stream.atEnd())
        {
            this->local_objects_ << IpfsHash(stream.readLine());
        }

        valid_data_ = true;
        network_reply->deleteLater();
    });
}

RefsReply::~RefsReply()
{
    int nb = refs.count();
    for(int i = 0; i < nb; i++)
    {
        delete refs[i];
    }
}
