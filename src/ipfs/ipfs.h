#ifndef IPFS_H
#define IPFS_H

#include <QObject>
#include <QQueue>
#include <QProcess>
#include <QSet>
#include "ipfs/ipfsadd.h"
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

class QHttpMultiPart;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QIODevice;
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
    QHttpMultiPart  *multipart;
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
    IpfsAdd add;
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
     * @param multipart If not null, send a post request with the data provided.
     */
    IpfsAccess * query(const QUrl &url, QHttpMultiPart *multipart = NULL);

    /**
     * @return true is the daemon is online and ready.
     */
    bool online() const;

    /**
     * @return true if the block is stored locally
     */
    bool is_object_local(const IpfsHash &hash) const;

signals:
    void objectAdded(const IpfsHash& hash);
    void objectRemoved(const IpfsHash& hash);

private:
    void init_commands();
    void launch_daemon();
    void launch_access(IpfsAccess* access);
    void on_online();
    void request_logs();

private slots:
    void daemon_started();
    void daemon_error(QProcess::ProcessError error);
    void daemon_finished(int exit_code, QProcess::ExitStatus exit_status);
    void daemon_stdout();
    void daemon_stderr();
    void daemon_logevent();

private:
    IpfsState state_;
    QNetworkAccessManager *manager_;
    QProcess *daemon_process_;
    QProcess *cli_process_;
    QString api_ip_;
    QString api_port_;
    QQueue<IpfsAccess*> access_buffer_;
    QSet<const IpfsHash> local_objects_;
    QNetworkReply *log_reply_;
};

#endif // IPFS_H
