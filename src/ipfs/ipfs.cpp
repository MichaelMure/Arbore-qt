#include "ipfs.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QUrl>
#include <QProcess>
#include <QRegExp>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QHttpMultiPart>

#include "directory.h"

/*
 * PlanUML
 * @startuml
 *
 * [*] --> LAUNCH_DAEMON
 * LAUNCH_DAEMON --> RUNNING : read stdin
 * RUNNING --> LAUNCH_DAEMON : crash
 * RUNNING --> QUITTING
 *
 * @enduml
 */

Q_GLOBAL_STATIC(Ipfs, singleton)

enum IpfsState : short
{
    LAUNCH_DAEMON,
    RUNNING,
    QUITTING
};

Ipfs::Ipfs()
    : state_(LAUNCH_DAEMON),
      manager_(new QNetworkAccessManager()),
      daemon_process_(NULL),
      cli_process_(NULL),
      api_ip_("127.0.0.1"),
      api_port_("5001"),
      log_reply_(NULL)
{
    // We are doing localhost request, bypass the non-internet connectivity blocking
    manager_->setNetworkAccessible(QNetworkAccessManager::Accessible);
    launch_daemon();
}

Ipfs::~Ipfs()
{
    this->state_ = QUITTING;

    if(log_reply_)
    {
        log_reply_->abort();
        log_reply_->deleteLater();
    }

    if(daemon_process_)
    {
        daemon_process_->terminate();
        if(!daemon_process_->waitForFinished())
        {
            daemon_process_->kill();
        }
        daemon_process_->deleteLater();
    }
    manager_->deleteLater();
}

Ipfs *Ipfs::instance()
{
    return singleton();
}

QUrl Ipfs::api_url(const QString &command)
{
    return QUrl(QString("http://%1:%2/api/v0/%3")
                .arg(api_ip_, api_port_, command));
}

IpfsAccess *Ipfs::query(const QUrl &url, QHttpMultiPart *multipart)
{
    IpfsAccess *access = new IpfsAccess();
    access->multipart = multipart;
    access->timer = new QElapsedTimer();
    access->request = new QNetworkRequest(url);

    // parent for cleaning
    if(multipart)
    {
        multipart->setParent(access);
    }

    // If not connected to the daemon, pill up the requests in a buffer
    if(online())
        launch_access((access));
    else
        access_buffer_ << access;

    return access;
}

bool Ipfs::online() const
{
    return state_ == RUNNING;
}

bool Ipfs::is_object_local(const IpfsHash &hash) const
{
    return local_objects_.contains(hash);
}

void Ipfs::init_commands()
{
    id.init();
    stats.init();
    swarm.init();
    version.init();
}

void Ipfs::launch_daemon()
{
    state_ = LAUNCH_DAEMON;

    // Add a custom repo location
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/repo");
    qDebug() << "Repo path: " << dir.absolutePath();

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("IPFS_PATH", dir.absolutePath());

    if(cli_process_ == NULL)
    {
        cli_process_ = new QProcess();
        cli_process_->setProcessEnvironment(env);
    }

    // Initialize the repo if needed
    if(!dir.exists())
    {
        dir.mkpath(".");

        // ipfs should be in the PATH
        cli_process_->start("ipfs", QStringList() << "init");
        cli_process_->waitForFinished();

        qDebug() << "Repo initialized";
    }

    // Configure the HTTP API addresse
    cli_process_->start("ipfs", QStringList() << "config" << "Addresses.API" << "/ip4/127.0.0.1/tcp/4280");
    cli_process_->waitForFinished();
    api_port_ = "4280";
    qDebug() << "HTTP API set to /ip4/127.0.0.1/tcp/4280";

    // Disable the gateway
    cli_process_->start("ipfs", QStringList() << "config" << "Addresses.Gateway" << "");
    cli_process_->waitForFinished();
    qDebug() << "Disabled gateway";

    // Add the API url to the CORS config to avoid 403
    cli_process_->start("ipfs",
        QStringList() << "config"
            << "--json"
            << "API.HTTPHeaders"
            << "{\"Access-Control-Allow-Origin\" : [\"http://127.0.0.1\", \"http://127.0.0.1:4280\"]}");
    cli_process_->waitForFinished();
    qDebug() << "Configured CORS";

    daemon_process_ = new QProcess(this);

    connect(daemon_process_, SIGNAL(started()),
            this, SLOT(daemon_started()));
    connect(daemon_process_, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(daemon_error(QProcess::ProcessError)));
    connect(daemon_process_, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(daemon_finished(int,QProcess::ExitStatus)));
    connect(daemon_process_, SIGNAL(readyReadStandardOutput()),
            this, SLOT(daemon_stdout()));
    connect(daemon_process_, SIGNAL(readyReadStandardError()),
            this, SLOT(daemon_stderr()));

    // ipfs should be in the PATH
    daemon_process_->setProcessEnvironment(env);
    daemon_process_->start("ipfs", QStringList() << "daemon");
}

void Ipfs::launch_access(IpfsAccess *access)
{
    access->timer->start();

    if(access->multipart)
    {
        access->reply = manager_->post(*access->request, access->multipart);
    }
    else
    {
        access->reply = manager_->get(*access->request);
    }

    connect(access->reply, &QNetworkReply::finished,
            this, [this, access]()
    {
        if(access->reply->error())
        {
            qDebug() << "http error: " << access->reply->errorString() << endl;
            qDebug() << "request: " << access->request->url() << endl;

            if(this->state_ == RUNNING)
            {
                // TODO: restart process ?
            }

            delete access;

            return;
        }

        QString url = access->request->url().toString().remove(QRegExp("^.*/v0/"));
        qDebug() << url << access->timer->elapsed() << "ms";

        emit access->finished();
    });
}

void Ipfs::on_online()
{
    // Configure the various log level we need
    cli_process_->start("ipfs", QStringList() << "log" << "level" << "all" << "info");
    cli_process_->waitForFinished();

    init_commands();

    // Refresh local objects
    RefsReply *refs_reply = refs.local();
    connect(refs_reply, &RefsReply::finished,
            this, [this, refs_reply]()
    {
        qDebug() << refs_reply->refs.size() << " obj";

        foreach (IpfsHash hash, refs_reply->refs)
        {
            if(this->local_objects_.contains(hash))
            {
                this->local_objects_.remove(hash);
            }
            else
            {
                emit objectAdded(hash);
            }
        }

        foreach (IpfsHash hash, this->local_objects_)
        {
            emit objectRemoved(hash);
        }

        this->local_objects_ = refs_reply->refs;
    });

    request_logs();

    while(!access_buffer_.empty())
    {
        launch_access(access_buffer_.dequeue());
    }
}

void Ipfs::request_logs()
{
    if(log_reply_)
    {
        log_reply_->deleteLater();
    }

    QUrl url = api_url("log/tail");
    QNetworkRequest request(url);

    log_reply_ = manager_->get(request);

    connect(log_reply_, SIGNAL(readyRead()),
            this, SLOT(daemon_logevent()));
}

void Ipfs::daemon_started()
{
    qDebug() << "daemon started";
}

void Ipfs::daemon_error(QProcess::ProcessError error)
{
    qDebug() << "daemon error " << error;
    if(state_ == RUNNING && daemon_process_->state() == QProcess::NotRunning)
    {
        // TODO: restart process
    }
}

void Ipfs::daemon_finished(int exit_code, QProcess::ExitStatus exit_status)
{
    qDebug() << "daemon finished exit code: " << exit_code
             << "exit status: " << exit_status;

    if(state_ == RUNNING && daemon_process_->state() == QProcess::NotRunning)
    {
        // TODO: restart process
    }
}

void Ipfs::daemon_stdout()
{
    QByteArray raw = daemon_process_->readAllStandardOutput();
    QTextStream stdout(&raw);

    while (!stdout.atEnd())
    {
        QString line = stdout.readLine();

        qDebug() << line;

        if(state_ == LAUNCH_DAEMON)
        {
            QRegExp regex("^Daemon is ready$");
            if(regex.indexIn(line) >= 0)
            {
                state_ = RUNNING;
                on_online();
            }
        }
    }
}

// Ipfs daemon output its log on stderr
void Ipfs::daemon_stderr()
{
    QByteArray raw = daemon_process_->readAllStandardError();
    QTextStream stderr(&raw);

    while (!stderr.atEnd())
    {
        QString line = stderr.readLine();
        qDebug() << line;
    }
}

void Ipfs::daemon_logevent()
{
    QByteArray raw = log_reply_->readAll();
    QTextStream log(&raw);

    while(!log.atEnd())
    {
        QString line = log.readLine();

        if(line.contains("Added block Begin"))
        {
            QJsonObject json = QJsonDocument::fromJson(line.toUtf8()).object();
            try
            {
                const IpfsHash hash(json.value("key").toString());
                local_objects_.insert(hash);
                emit objectAdded(hash);
                qDebug() << "added " << hash;
            }
            catch(std::exception& e)
            {
                qDebug() << e.what() << line;
            }
        }

        if(line.contains("Removed block"))
        {
            QJsonObject json = QJsonDocument::fromJson(line.toUtf8()).object();
            try
            {
                const IpfsHash hash(json.value("key").toString());
                local_objects_.remove(hash);
                qDebug() << "removed" << hash;
                emit objectRemoved(hash);
            }
            catch(std::exception& e)
            {
                qDebug() << e.what() << line;
            }
        }
    }
}

IpfsAccess::~IpfsAccess()
{
    delete this->request;
    this->reply->deleteLater();
    delete this->timer;
}

const QJsonObject IpfsAccess::json()
{
    QString str = this->reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    return doc.object();
}
