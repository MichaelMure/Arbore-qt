#ifndef IPFSHASH_H
#define IPFSHASH_H

#include <QString>
#include <QHash>

/**
 * @brief The IpfsHash class hold an IPFS multi-hash
 * https://github.com/jbenet/multihash/
 */
class IpfsHash
{
public:
    IpfsHash();
    IpfsHash(QString hash);
    IpfsHash(const IpfsHash &hash);
    virtual ~IpfsHash() {}

    IpfsHash& operator=(const IpfsHash& other);
    bool operator==(const IpfsHash& other) const;
    const QString& ToString() const;
    operator QString() const;

private:
    QString str_;
};

inline uint qHash(const IpfsHash &key, uint seed)
{
    return qHash(key.ToString(), seed);
}

#endif // IPFSHASH_H
