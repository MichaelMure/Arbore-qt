#ifndef IPFSREPO_H
#define IPFSREPO_H

#include <QObject>

class IpfsRepo : public QObject
{
    Q_OBJECT
public:
    explicit IpfsRepo(QObject *parent = 0);
    virtual ~IpfsRepo() {}

    void gc();
};

#endif // IPFSREPO_H
