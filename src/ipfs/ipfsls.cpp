#include "ipfsls.h"
#include "ipfs/ipfs.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>

const QString API_COMMAND = "ls";

IpfsLs::IpfsLs(QObject *parent)
    : QObject(parent)
{
}

LsReply * IpfsLs::ls(const IpfsHash &hash)
{
    QUrl url = Ipfs::instance().api_url(API_COMMAND);
    QUrlQuery query;

    query.addQueryItem("arg", hash.ToString());

    url.setQuery(query);
    QNetworkReply *network_reply = Ipfs::instance().manual_query(url);

    LsReply *ls_reply = new LsReply();

    connect(network_reply, SIGNAL(finished()),
            ls_reply, SLOT(replyFinished()));

    return ls_reply;
}

LsReply::~LsReply()
{
    int nb = entries.count();
    for(int i = 0; i < nb; i++)
    {
        delete entries[i];
    }
}

void LsReply::replyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if(reply->error())
    {
        qDebug() << "http error: " << reply->errorString() << endl;
        qDebug() << "request: " << reply->request().url() << endl;
        return;
    }

    QString str = reply->readAll();
//    qDebug() << "http reply : " << str;

    const QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    const QJsonObject json = doc.object();

    const QJsonValue links = json.value("Objects").toArray().first().toObject().value("Links");

    foreach (const QJsonValue &link, links.toArray())
    {
        LsEntry *entry = new LsEntry(this);

        QJsonObject link_obj = link.toObject();

        entry->name_ = new QString(link_obj["Name"].toString());
        entry->hash_ = new IpfsHash(link_obj["Hash"].toString());
        entry->size_ = link_obj["Size"].toInt();
        entry->type_ = (LsEntry::LsEntryType) link_obj["Type"].toInt();

        entries << entry;
    }

    emit finished();

    reply->deleteLater();
}


LsEntry::LsEntry(QObject *parent)
    : QObject(parent)
{
}

QString &LsEntry::name() const
{
    return *name_;
}

IpfsHash &LsEntry::hash() const
{
    return *hash_;
}

uint LsEntry::size() const
{
    return size_;
}

LsEntry::LsEntryType LsEntry::type() const
{
    return type_;
}

