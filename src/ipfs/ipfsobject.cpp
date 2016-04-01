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

HashReply *IpfsObject::new_dir(const QList<std::pair<const QString, const IpfsHash>> &hashes)
{
    HashReply *result = new HashReply();

    // get a new empty ipfs object
    HashReply *new_obj_reply = new_object();
    connect(new_obj_reply, &HashReply::finished,
            this, [this, new_obj_reply, hashes, result]()
    {
        IpfsHash root = new_obj_reply->hash;

        // return if all links added
        if(hashes.length() == 0)
        {
            result->hash = root;
            emit result->finished();
            delete new_obj_reply;
            return;
        }

        HashReply* add_recurs_reply = add_link_recurs(root, hashes, 0);
        connect(add_recurs_reply, &HashReply::finished,
                this, [result, add_recurs_reply, new_obj_reply]()
        {
            result->hash = add_recurs_reply->hash;
            emit result->finished();
            delete add_recurs_reply;
            delete new_obj_reply;
        });
    });

    return result;
}

HashReply *IpfsObject::new_object()
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND + "/new");
    IpfsAccess *access = Ipfs::instance()->query(url);

    HashReply *reply = new HashReply();

    connect(access, &IpfsAccess::finished,
            this, [access, reply]()
    {
        const QJsonObject json = access->json();
        reply->hash = IpfsHash(json.value("Hash").toString());

        emit reply->finished();

        delete access;
    });

    return reply;
}

HashReply *IpfsObject::add_link(const IpfsHash &root, const QString name, const IpfsHash &ref)
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND + "/patch/add-link");
    QUrlQuery query;

    // argument (not options) are passed as "arg" in the api
    query.addQueryItem("arg", root.ToString());
    query.addQueryItem("arg", name);
    query.addQueryItem("arg", ref.ToString());

    url.setQuery(query);
    IpfsAccess *access = Ipfs::instance()->query(url);

    HashReply *reply = new HashReply();

    connect(access, &IpfsAccess::finished,
            this, [access, reply]()
    {
        const QJsonObject json = access->json();
        reply->hash = IpfsHash(json.value("Hash").toString());

        emit reply->finished();

        delete access;
    });

    return reply;
}

HashReply *IpfsObject::add_link_recurs(
        const IpfsHash &root,
        const QList<std::pair<const QString, const IpfsHash>> &hashes,
        int index)
{
    HashReply *result = new HashReply();

    if(hashes.length() > 1)
    {
        std::pair<const QString, const IpfsHash> pair = hashes.first();
        hashes.removeFirst();

        HashReply *recurs_add_reply = add_link_recurs(root, hashes, index+1);
        connect(recurs_add_reply, &HashReply::finished,
                this, [this, result, recurs_add_reply, pair]()
        {
            HashReply *add_reply = add_link(recurs_add_reply->hash, pair.first, pair.second);
            connect(add_reply, &HashReply::finished,
                    this, [result, add_reply]()
            {
                result->hash = add_reply->hash;
                emit result->finished();
                delete add_reply;
            });
        });
    }
    else
    {
        std::pair<const QString, const IpfsHash> pair = hashes.first();
        HashReply *add_reply = add_link(root, pair.first, pair.second);

        connect(add_reply, &HashReply::finished,
                this, [result, add_reply]()
        {
            result->hash = add_reply->hash;
            emit result->finished();
            delete add_reply;
        });
    }

    return result;
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
