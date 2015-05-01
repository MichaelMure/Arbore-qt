#ifndef IPFS_H
#define IPFS_H

#include <QObject>
#include "ipfs/ipfspin.h"
#include "ipfs/ipfsswarm.h"
#include "ipfs/ipfsversion.h"

class QNetworkAccessManager;
class QNetworkReply;

class Ipfs : public QObject
{
    Q_OBJECT
public:
    // Singleton
    static Ipfs& instance();

    // IPFS API access
    IpfsPin pin;
    IpfsSwarm swarm;
    IpfsVersion version;

    void query(const QUrl &url, IpfsCommand *originator);

private:
    Ipfs() {}                    // hide constructor
    ~Ipfs();                     // hide destructor
    Ipfs(Ipfs const&);           // hide copy constructor
    void operator=(Ipfs const&); // hide assign op

    void init();

private slots:
    void replyFinished (QNetworkReply *reply);

private:
    QNetworkAccessManager* manager;
};

#endif // IPFS_H
