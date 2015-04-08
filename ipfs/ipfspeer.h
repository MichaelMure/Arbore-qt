#ifndef IPFSPEER_H
#define IPFSPEER_H

#include <QString>

class IpfsPeer : private QString
{
   // Q_OBJECT
public:
    IpfsPeer(QString id) : QString(id) {}
    virtual ~IpfsPeer() {}

    QString ToString() const { return *this; }
};

#endif // IPFSPEER_H
