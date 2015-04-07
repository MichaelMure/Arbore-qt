#ifndef IPFSHASH_H
#define IPFSHASH_H

#include <QString>

class IpfsHash : private QString
{
public:
    IpfsHash(QString hash) : QString(hash) {}

    QString ToString() const { return *this; }
};

#endif // IPFSHASH_H
