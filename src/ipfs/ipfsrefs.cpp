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
    : QObject(parent),
      valid_data_(false),
      local_objects_()
{
    startTimer(10 * 1000); // 10s
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

            refs_reply->refs << new IpfsHash(json.value("Ref").toString().trimmed());
        }

        emit refs_reply->finished();

        delete access;
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

void IpfsRefs::timerEvent(QTimerEvent *)
{
    if(Ipfs::instance()->online())
        refresh_objects();
}

void IpfsRefs::refresh_objects()
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND + "/local");

    IpfsAccess *access = Ipfs::instance()->query(url);

    connect(access, &IpfsAccess::finished,
            this, [this, access]()
    {
        QTextStream stream(access->reply->readAll());

        QSet<IpfsHash> objects = QSet<IpfsHash>();

        while(!stream.atEnd())
        {
            objects << IpfsHash(stream.readLine());
        }

        qDebug() << objects.size() << " obj";

        foreach (IpfsHash hash, objects)
        {
            if(this->local_objects_.contains(hash))
            {
                this->local_objects_.remove(hash);
            }
            else
            {
                emit objectAdded(hash);
            }
        }

        foreach (IpfsHash hash, this->local_objects_)
        {
            emit objectRemoved(hash);
        }

        this->local_objects_ = objects;

        valid_data_ = true;

        delete access;
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


