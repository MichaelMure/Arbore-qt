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
      api_ip_("127.0.0.1"),
      api_port_("5001")
{
    launch_daemon();
}

Ipfs::~Ipfs()
{
    this->state_ = QUITTING;
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

IpfsAccess *Ipfs::query(const QUrl &url)
{
    IpfsAccess *access = new IpfsAccess();
    access->timer = new QElapsedTimer();
    access->request = new QNetworkRequest(url);

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

void Ipfs::init_commands()
{
    id.init();
    refs.init();
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

    // Initialize the repo if needed
    if(!dir.exists())
    {
        dir.mkpath(".");

        // ipfs should be in the PATH
        QProcess process;
        process.setProcessEnvironment(env);
        process.start("ipfs", QStringList() << "init");
        process.waitForFinished();

        qDebug() << "Repo initialized";
    }

    // Configure the HTTP API addresse
    QProcess process;
    process.setProcessEnvironment(env);
    process.start("ipfs", QStringList() << "config" << "Addresses.API" << "/ip4/127.0.0.1/tcp/4280");
    process.waitForFinished();
    api_port_ = "4280";
    qDebug() << "HTTP API set to /ip4/127.0.0.1/tcp/4280";

    // Configure the various log level we need
    process.start("ipfs", QStringList() << "log" << "level" << "all" << "panic");
    process.waitForFinished();

    process.start("ipfs", QStringList() << "log" << "level" << "all" << "panic");
    process.waitForFinished();

    process.start("ipfs", QStringList() << "log" << "level" << "all" << "panic");
    process.waitForFinished();



    daemon_process_ = new QProcess(this);

    connect(daemon_process_, SIGNAL(started()),
            this, SLOT(daemon_started()));
    connect(daemon_process_, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(daemon_error(QProcess::ProcessError)));
    connect(daemon_process_, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(daemon_finished(int,QProcess::ExitStatus)));
    connect(daemon_process_, SIGNAL(readyRead()),
            this, SLOT(daemon_stdout()));

    // ipfs should be in the PATH
    daemon_process_->setProcessEnvironment(env);
    daemon_process_->start("ipfs", QStringList() << "daemon");
}

void Ipfs::launch_access(IpfsAccess *access)
{
    access->timer->start();
    access->reply = manager_->get(*access->request);

    connect(access->reply, &QNetworkReply::finished,
            this, [this, access]()
    {
        if(access->reply->error())
        {
            qDebug() << "http error: " << access->reply->errorString() << endl;
            qDebug() << "request: " << access->request->url() << endl;

            if(this->state_ == RUNNING)
            {
                // TODO: restart process
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
    init_commands();

    while(!access_buffer_.empty())
    {
        launch_access(access_buffer_.dequeue());
    }
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
    QByteArray stdout_ = daemon_process_->readAllStandardOutput();

    if(state_ == LAUNCH_DAEMON)
    {
        QRegExp regex("Daemon is ready");
        if(regex.indexIn(stdout_) >= 0)
        {
            state_ = RUNNING;
            on_online();
        }
    }

    qDebug() << stdout_;
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
