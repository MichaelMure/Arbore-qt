#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>
#include "ipfs/ipfshash.h"

class Object : public QObject
{
    Q_OBJECT
public:
    Object(const IpfsHash &hash);
    Object(QString hash);
    virtual ~Object() {}

    const IpfsHash& hash() const;

private:
    IpfsHash hash_;
};

#endif // OBJECT_H
