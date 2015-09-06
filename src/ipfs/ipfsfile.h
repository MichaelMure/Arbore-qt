#ifndef IPFSFILE_H
#define IPFSFILE_H

#include <QObject>
#include "ipfs/ipfshash.h"
#include "ipfs/ipfsls.h"
#include "object.h"

class FileReply;

class IpfsFile : public QObject
{
    Q_OBJECT
public:
    explicit IpfsFile(QObject *parent = 0);
    virtual ~IpfsFile() {}

public:
    FileReply *ls(const IpfsHash &hash);
};


class FileReply : public QObject
{
    Q_OBJECT
public:
    virtual ~FileReply();
signals:
    void finished();
public:
    IpfsHash hash;
    uint size;
    Object::ObjectType type;
    QList<const LsEntry*> entries;
};

#endif // IPFSFILE_H
