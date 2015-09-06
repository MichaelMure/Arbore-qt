#ifndef IPFSOBJECT_H
#define IPFSOBJECT_H

#include <QObject>
#include "ipfs/ipfshash.h"

class ObjectLinkReply;
class ObjectLinkEntry;

class IpfsObject : public QObject
{
    Q_OBJECT
public:
    explicit IpfsObject(QObject *parent = 0);
    virtual ~IpfsObject() {}

    ObjectLinkReply* links(const IpfsHash &hash);
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
