#ifndef IPFSHASH_H
#define IPFSHASH_H

#include <QString>

/**
 * @brief The IpfsHash class hold an IPFS multi-hash
 * https://github.com/jbenet/multihash/
 */
class IpfsHash : private QString
{
    //Q_OBJECT
public:
    IpfsHash(QString hash) : QString(hash) {}
    virtual ~IpfsHash() {}

    QString ToString() const { return *this; }
};

#endif // IPFSHASH_H
