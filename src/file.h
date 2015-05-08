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
    File(const IpfsHash &hash);
    virtual ~File();

private:
    QHash<IpfsHash, Block*> blocks_;
};

#endif // FILE_H
