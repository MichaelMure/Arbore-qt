#include "ipfsobject.h"
#include "ipfs/ipfs.h"

#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>

const QString API_COMMAND = "object";

IpfsObject::IpfsObject(QObject *parent) : QObject(parent)
{
}

ObjectLinkReply *IpfsObject::links(const IpfsHash &hash)
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND + "/links");
    QUrlQuery query;

    query.addQueryItem("arg", hash.ToString());

    url.setQuery(query);
    IpfsAccess *access = Ipfs::instance()->query(url);

    ObjectLinkReply *reply = new ObjectLinkReply();

    connect(access, &IpfsAccess::finished,
            this, [access, reply]()
    {
        const QJsonObject json = access->json();
        const QJsonValue links = json.value("Links");

        foreach (const QJsonValue &link, links.toArray())
        {
            ObjectLinkEntry *entry = new ObjectLinkEntry(reply);

            QJsonObject link_obj = link.toObject();

            entry->name_ = new QString(link_obj["Name"].toString());
            entry->hash_ = new IpfsHash(link_obj["Hash"].toString());
            entry->size_ = link_obj["Size"].toInt();

            reply->entries << entry;
        }

        emit reply->finished();

        delete access;
    });

    return reply;
}

ObjectLinkReply::~ObjectLinkReply()
{
    int nb = entries.count();
    for(int i = 0; i < nb; i++)
    {
        delete entries[i];
    }
}

ObjectLinkEntry::ObjectLinkEntry(QObject *parent)
    : QObject(parent),
      name_(NULL),
      hash_(NULL),
      size_(-1)
{
}

ObjectLinkEntry::~ObjectLinkEntry()
{
    delete name_;
    delete hash_;
}

QString &ObjectLinkEntry::name() const
{
    return *name_;
}

IpfsHash &ObjectLinkEntry::hash() const
{
    return *hash_;
}

uint ObjectLinkEntry::size() const
{
    return size_;
}
