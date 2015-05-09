#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "object.h"
#include "ipfs/ipfshash.h"

#include <QHash>

class LsEntry;
struct Child;

class Directory : public Object
{
    Q_OBJECT
public:
    Directory(const IpfsHash &hash);
    Directory(const QString &hash);
    virtual ~Directory();

    // Object interface
public:
    uint size_total() const;
    uint size_local() const;
    uint block_total() const;
    uint block_local() const;
    uint file_total() const;
    uint file_local() const;

private:
    QHash<IpfsHash, Child *> child_hashes_;
    QHash<QString, Child *> child_names_;
};

#endif // DIRECTORY_H
