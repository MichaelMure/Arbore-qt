#ifndef IPFSPEER_H
#define IPFSPEER_H

#include <QString>

class IpfsPeer : private QString
{
public:
    IpfsPeer(QString id) : QString(id) {}

    QString ToString() const { return *this; }
};

#endif // IPFSPEER_H
