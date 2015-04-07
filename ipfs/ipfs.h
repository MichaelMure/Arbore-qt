#ifndef IPFS_H
#define IPFS_H

#include <QObject>
#include "ipfs/ipfspin.h"
#include "ipfs/ipfsversion.h"

class QNetworkAccessManager;
class QNetworkReply;

class Ipfs : public QObject
{
    Q_OBJECT
public:
    // Singleton
    static Ipfs& instance();

    IpfsPin pin;
    IpfsVersion version;

    void query(QUrl &url, IpfsCommand *originator);

private:
    Ipfs() {}
    ~Ipfs();
    Ipfs(Ipfs const&);
    void operator=(Ipfs const&);

    void init();

private slots:
    void replyFinished (QNetworkReply *reply);

private:
    QNetworkAccessManager* manager;
};

#endif // IPFS_H
