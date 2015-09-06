#include "ipfsfile.h"
#include "ipfs/ipfs.h"
#include "object.h"

#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>

const QString API_COMMAND = "file";

IpfsFile::IpfsFile(QObject *parent)
    : QObject(parent)
{
}

FileReply *IpfsFile::ls(const IpfsHash &hash)
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND + "/ls");
    QUrlQuery query;

    query.addQueryItem("arg", hash.ToString());

    url.setQuery(query);
    IpfsAccess *access = Ipfs::instance()->query(url);

    FileReply *reply = new FileReply();

    connect(access, &IpfsAccess::finished,
            this, [access, reply, hash]()
    {
        const QJsonObject json = access->json();
        const QJsonValue object = json.value("Objects");
        const QJsonObject obj = object.toObject().find(hash.ToString()).value().toObject();

        reply->hash = hash;
        reply->size = obj.value("Size").toInt();
        reply->type = Object::decode_type(obj.value("Type").toString());

        const QJsonValue links = obj.value("Links");

        foreach (const QJsonValue &link, links.toArray())
        {
            LsEntry *entry = new LsEntry(reply);

            QJsonObject link_obj = link.toObject();

            entry->name_ = new QString(link_obj["Name"].toString());
            entry->hash_ = new IpfsHash(link_obj["Hash"].toString());
            entry->size_ = link_obj["Size"].toInt();
            entry->type_ = Object::decode_type(link_obj["Type"].toString());

            reply->entries << entry;
        }

        emit reply->finished();

        delete access;
    });

    return reply;
}

FileReply::~FileReply()
{
    int nb = entries.count();
    for(int i = 0; i < nb; i++)
    {
        delete entries[i];
    }
}
