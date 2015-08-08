#ifndef IPFSGET_H
#define IPFSGET_H

#include <QObject>
#include "ipfs/ipfshash.h"

class IpfsGet : public QObject
{
    Q_OBJECT
public:
    explicit IpfsGet(QObject *parent = 0);
    virtual ~IpfsGet() {}

    /* Will not work, https://github.com/ipfs/go-ipfs/issues/1210 */
    // Commented as it's likely to be in a different way
    // See https://github.com/ipfs/go-ipfs/issues/875
    //void write_on_disk(const IpfsHash &hash, const QString &path);
};

#endif // IPFSGET_H
