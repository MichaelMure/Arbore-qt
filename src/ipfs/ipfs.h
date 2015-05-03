#ifndef IPFS_H
#define IPFS_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include "ipfs/ipfsid.h"
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
    IpfsId id;
    IpfsPin pin;
    IpfsSwarm swarm;
    IpfsVersion version;

    void query(const QUrl &url, AbstractIpfsCommand *originator);

private:
    Ipfs();                      // hide constructor
    ~Ipfs();                     // hide destructor
    Ipfs(Ipfs const&);           // hide copy constructor
    void operator=(Ipfs const&); // hide assign op

    void init();
    void init_commands();
    void launch_daemon();

private slots:
    void replyFinished (QNetworkReply *reply);
    void daemon_started();
    void daemon_error(QProcess::ProcessError error);
    void daemon_finished(int exit_code, QProcess::ExitStatus exit_status);
    void timer();


private:
    enum IpfsState { PING_DAEMON, LAUNCH_DAEMON, RUNNING };
    IpfsState state_;
    QNetworkAccessManager *manager_;
    QProcess *daemon_process_;
    QTimer refreshTimer_;
};

#endif // IPFS_H
