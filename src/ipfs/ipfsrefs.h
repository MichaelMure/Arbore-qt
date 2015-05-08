#ifndef IPFSREFS_H
#define IPFSREFS_H

#include <QObject>
#include "ipfs/ipfshash.h"

class RefsReply;

class IpfsRefs : public QObject
{
    Q_OBJECT
public:
    explicit IpfsRefs(QObject *parent = 0);
    virtual ~IpfsRefs() {}

public:
    RefsReply* recursive_refs(const IpfsHash &hash);
};


class RefsReply : public QObject
{
    Q_OBJECT
public:
    virtual ~RefsReply();
signals:
    void finished();

public:
    QList<const IpfsHash*> refs;
};


#endif // IPFSREFS_H
