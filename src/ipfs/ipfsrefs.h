#ifndef IPFSREFS_H
#define IPFSREFS_H

#include <QObject>
#include "ipfs/ipfshash.h"

#include <QSet>

class RefsReply;

class IpfsRefs : public QObject
{
    Q_OBJECT
public:
    explicit IpfsRefs(QObject *parent = 0);
    virtual ~IpfsRefs() {}

    RefsReply* recursive_refs(const IpfsHash &hash) const;
    RefsReply* local() const;
};

class RefsReply : public QObject
{
    Q_OBJECT
signals:
    void finished();

public:
    QSet<const IpfsHash> refs;
};


#endif // IPFSREFS_H
