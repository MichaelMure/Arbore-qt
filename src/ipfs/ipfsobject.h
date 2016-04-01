#ifndef IPFSOBJECT_H
#define IPFSOBJECT_H

#include <QObject>
#include "ipfs/ipfshash.h"
#include "ipfs/hashreply.h"

class ObjectLinkReply;
class ObjectLinkEntry;
class NewObjectReply;

class IpfsObject : public QObject
{
    Q_OBJECT
public:
    explicit IpfsObject(QObject *parent = 0);
    virtual ~IpfsObject() {}

    ObjectLinkReply* links(const IpfsHash &hash);

    // Create a new dir in IPFS with the given name-->refs links
    HashReply* new_dir(const QList<std::pair<const QString, const IpfsHash>> &hashes);

private:
    // Add a new empty dir to IPFS
    HashReply* new_object();

    // Add a new link to the root object
    HashReply* add_link(const IpfsHash& root, const QString name, const IpfsHash& ref);

    // Recursively add the links to the root objects
    HashReply* add_link_recurs(const IpfsHash &root,
                               const QList<std::pair<const QString, const IpfsHash> > &hashes,
                               int index);
};

class ObjectLinkReply : public QObject
{
    Q_OBJECT
public:
    virtual ~ObjectLinkReply();
signals:
    void finished();

public:
    QList<const ObjectLinkEntry*> entries;
};

class ObjectLinkEntry : public QObject
{
    friend class IpfsObject;
    Q_OBJECT
public:
    explicit ObjectLinkEntry(QObject *parent = 0);
    virtual ~ObjectLinkEntry();

    QString &name() const;
    IpfsHash& hash() const;
    uint size() const;

private:
    QString *name_;
    IpfsHash *hash_;
    uint size_;
};

#endif // IPFSOBJECT_H
