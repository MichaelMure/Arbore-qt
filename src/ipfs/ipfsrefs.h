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
    bool is_object_local(const IpfsHash &hash) const;

public:
    void init();
    bool valid_data() const;

signals:
    void objectAdded(const IpfsHash& hash);
    void objectRemoved(const IpfsHash& hash);

private:
    bool valid_data_;
    QSet<IpfsHash> local_objects_;

private:
    void timerEvent(QTimerEvent *);
    void refresh_objects();
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
