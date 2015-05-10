#ifndef IPFS_H
#define IPFS_H

#include <QObject>
#include <QMap>
#include <QProcess>
#include <QTimer>
#include "ipfs/ipfsget.h"
#include "ipfs/ipfsid.h"
#include "ipfs/ipfsls.h"
#include "ipfs/ipfspin.h"
#include "ipfs/ipfsrefs.h"
#include "ipfs/ipfsstats.h"
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
    IpfsGet get;
    IpfsId id;
    IpfsLs ls;
    IpfsPin pin;
    IpfsRefs refs;
    IpfsStats stats;
    IpfsSwarm swarm;
    IpfsVersion version;

    /**
     * @return the API url for the specified command
     */
    QUrl api_url(const QString &command);

    /**
     * Query the API with only a command name
     * @param command
     * @param originator
     */
    void query(const QString &command, IApiListener *listener = NULL);

    /**
     * Query a specific API url, constructed from api_url()
     * @param url
     * @param originator
     */
    void query(const QUrl &url, IApiListener *listener = NULL);

    /**
     * Query the API with only a command name
     * @param command
     */
    QNetworkReply * manual_query(const QString &command);

    /**
     * Query a specific API url, constructed from api_url()
     * @param url
     */
    QNetworkReply * manual_query(const QUrl &url);

private:
    Ipfs();                      // hide constructor
    ~Ipfs();                     // hide destructor
    Ipfs(Ipfs const&);           // hide copy constructor
    void operator=(Ipfs const&); // hide assign op

    void init();
    void init_commands();
    void launch_daemon();

private slots:
    void replyFinished ();
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
    QString api_ip_;
    QString api_port_;
    QMap<QNetworkReply*, IApiListener*> replies_listener;
};

#endif // IPFS_H
