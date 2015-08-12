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
    IpfsAccess *access = Ipfs::instance().query(url);

    LsReply *ls_reply = new LsReply();

    connect(access, &IpfsAccess::finished,
            this, [access, ls_reply]()
    {
        QString str = access->reply->readAll();

        const QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
        const QJsonObject json = doc.object();

        const QJsonValue links = json.value("Objects").toArray().first().toObject().value("Links");

        foreach (const QJsonValue &link, links.toArray())
        {
            LsEntry *entry = new LsEntry(ls_reply);

            QJsonObject link_obj = link.toObject();

            entry->name_ = new QString(link_obj["Name"].toString());
            entry->hash_ = new IpfsHash(link_obj["Hash"].toString());
            entry->size_ = link_obj["Size"].toInt();
            entry->type_ = (Object::ObjectType) link_obj["Type"].toInt();

            ls_reply->entries << entry;
        }

        emit ls_reply->finished();

        delete access;
    });

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

LsEntry::LsEntry(QObject *parent)
    : QObject(parent),
      name_(NULL),
      hash_(NULL),
      size_(-1),
      type_(Object::INVALID)
{
}

LsEntry::~LsEntry()
{
    delete name_;
    delete hash_;
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

Object::ObjectType LsEntry::type() const
{
    return type_;
}
