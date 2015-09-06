#ifndef IPFSLS_H
#define IPFSLS_H

#include <QObject>
#include "ipfs/ipfshash.h"
#include "object.h"

class LsReply;
class LsEntry;

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
    IpfsHash hash;
    QList<const LsEntry*> entries;
};


class LsEntry : public QObject
{
    friend class IpfsLs;
    Q_OBJECT
public:
    explicit LsEntry(QObject *parent = 0);
    ~LsEntry();

public:
    QString& name() const;
    IpfsHash& hash() const;
    uint size() const;
    Object::ObjectType type() const;

private:
    QString *name_;
    IpfsHash *hash_;
    uint size_;
    Object::ObjectType type_;
};

#endif // IPFSLS_H
