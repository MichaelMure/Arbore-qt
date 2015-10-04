#ifndef IPFS_H
#define IPFS_H

#include <QObject>
#include <QQueue>
#include <QProcess>
#include "ipfs/ipfsget.h"
#include "ipfs/ipfsfile.h"
#include "ipfs/ipfsid.h"
#include "ipfs/ipfslog.h"
#include "ipfs/ipfsls.h"
#include "ipfs/ipfsobject.h"
#include "ipfs/ipfspin.h"
#include "ipfs/ipfsrefs.h"
#include "ipfs/ipfsrepo.h"
#include "ipfs/ipfsstats.h"
#include "ipfs/ipfsswarm.h"
#include "ipfs/ipfsversion.h"

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QElapsedTimer;
enum IpfsState : short;

class IpfsAccess : public QObject
{
    Q_OBJECT
public:
    virtual ~IpfsAccess();
    const QJsonObject json();
public:
    QNetworkRequest *request;
    QNetworkReply   *reply;
    QElapsedTimer   *timer;

signals:
    void finished();
};

class Ipfs : public QObject
{
    Q_OBJECT
public:
    Ipfs();
    virtual ~Ipfs();

    // Singleton
    static Ipfs *instance();

    // IPFS API access
    IpfsFile file;
    IpfsGet get;
    IpfsId id;
    IpfsLog log;
    IpfsLs ls;
    IpfsObject object;
    IpfsPin pin;
    IpfsRefs refs;
    IpfsRepo repo;
    IpfsStats stats;
    IpfsSwarm swarm;
    IpfsVersion version;

    /**
     * @return the API url for the specified command
     */
    QUrl api_url(const QString &command);

    /**
     * Query a specific API url, constructed from api_url()
     * @param url
     */
    IpfsAccess * query(const QUrl &url);

    /**
     * @return true is the daemon is online and ready.
     */
    bool online() const;

private:
    void init_commands();
    void launch_daemon();
    void launch_access(IpfsAccess* access);
    void on_online();

private slots:
    void daemon_started();
    void daemon_error(QProcess::ProcessError error);
    void daemon_finished(int exit_code, QProcess::ExitStatus exit_status);
    void daemon_stdout();

private:
    IpfsState state_;
    QNetworkAccessManager *manager_;
    QProcess *daemon_process_;
    QString api_ip_;
    QString api_port_;
    QQueue<IpfsAccess*> access_buffer_;
};

#endif // IPFS_H
