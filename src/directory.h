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
    virtual ~Directory();

private:
    QHash<IpfsHash, Child *> child_hashes_;
    QHash<QString, Child *> child_names_;
};

#endif // DIRECTORY_H
