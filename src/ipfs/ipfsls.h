#ifndef IPFSLS_H
#define IPFSLS_H

#include <QObject>
#include "ipfs/ipfshash.h"
#include "object.h"

class LsReply;
class LsEntry;
class IpfsAccess;

class IpfsLs : public QObject
{
    Q_OBJECT
public:
    explicit IpfsLs(QObject *parent = 0);
    virtual ~IpfsLs() {}

public:
    LsReply* ls(const IpfsHash &hash);
};


class LsReply : public QObject
{
    Q_OBJECT
public:
    virtual ~LsReply();
signals:
    void finished();

public:
    QList<const LsEntry*> entries;
};


class LsEntry : public QObject
{
    Q_OBJECT
public:
    explicit LsEntry(QObject *parent = 0);
    ~LsEntry();

    QString& name() const;
    IpfsHash& hash() const;
    uint size() const;
    Object::ObjectType type() const;

public:
    QString *name_;
    IpfsHash *hash_;
    uint size_;
    Object::ObjectType type_;
};

#endif // IPFSLS_H
