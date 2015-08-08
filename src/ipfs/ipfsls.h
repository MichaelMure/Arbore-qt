#ifndef IPFSLS_H
#define IPFSLS_H

#include <QObject>
#include "ipfs/ipfshash.h"

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
    enum LsEntryType
    {
        RAW = 0,
        DIRECTORY = 1,
        FILE = 2,
        METADATA = 3
    };

public:
    explicit LsEntry(QObject *parent = 0);
    ~LsEntry()
    {
        delete name_;
        delete hash_;
    }

    QString& name() const;
    IpfsHash& hash() const;
    uint size() const;
    LsEntryType type() const;

public:
    QString *name_;
    IpfsHash *hash_;
    uint size_;
    LsEntryType type_;
};

#endif // IPFSLS_H
