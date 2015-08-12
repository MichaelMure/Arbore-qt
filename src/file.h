#ifndef FILE_H
#define FILE_H

#include "object.h"
#include "ipfs/ipfshash.h"

#include <QHash>

struct Block;

class File : public Object
{
    Q_OBJECT
public:
    File(const IpfsHash &hash, const QString &name = "");
    File(const QString &hash, const QString &name = "");
    virtual ~File();

    // Object interface
public:
    ObjectType type() const;
    uint size_total() const;
    uint size_local() const;
    uint block_total() const;
    uint block_local() const;
    uint file_total() const;
    uint file_local() const;

private slots:
    void objectAdded(const IpfsHash& hash);
    void objectRemoved(const IpfsHash& hash);

private:
    QHash<IpfsHash, Block*> blocks_;
    uint size_;
};

#endif // FILE_H
