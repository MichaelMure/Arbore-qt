#include "ipfs.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <QProcess>
#include <QRegExp>

#include "directory.h"

static bool initialized = false;

Ipfs::Ipfs()
    : stats(),
      state_(PING_DAEMON),
      manager_(NULL),
      daemon_process_(NULL),
      api_ip_("127.0.0.1"),
      api_port_("5001")
{
}

Ipfs& Ipfs::instance()
{
    static Ipfs instance;
    if(!initialized)
    {
        initialized = true;
        instance.init();
    }
    return instance;
}

QUrl Ipfs::api_url(const QString &command)
{
    return QUrl(QString("http://%1:%2/api/v0/%3")
                .arg(api_ip_, api_port_, command));
}

IpfsAccess *Ipfs::query(const QUrl &url)
{
//    qDebug() << "HTTP query: " << url;
    IpfsAccess *access = new IpfsAccess();
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

void Ipfs::init()
{
    manager_ = new QNetworkAccessManager();
    daemon_process_ = NULL;

    // Ping the HTTP API to find out if a daemon is running
    state_ = PING_DAEMON;

    QUrl url = api_url("version");
    QNetworkRequest request = QNetworkRequest(url);
    QNetworkReply *reply = manager_->get(request);

    connect(reply, &QNetworkReply::finished,
            this, [this, reply]()
    {
        if(state_ == PING_DAEMON)
        {
            if(reply->error())
            {
                qDebug() << "Could not ping the API, lauching daemon manually";
                launch_daemon();
                return;
            }

            QString str = reply->readAll();
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &error);

            if(error.error != QJsonParseError::NoError)
            {
                qDebug() << "Error while pinging the API, lauching daemon manually";
                launch_daemon();
                return;
            }

            QJsonValue value = doc.object().value("Version");
            if(value == QJsonValue::Undefined)
            {
                qDebug() << "Unparsable json from API pinging, lauching daemon manually";
                launch_daemon();
                return;
            }

            // Daemon API should be OK at this point !
            state_ = RUNNING;
            init_commands();
        }

        reply->deleteLater();
    });
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
    daemon_process_ = new QProcess(this);

    connect(daemon_process_, SIGNAL(started()),
            this, SLOT(daemon_started()));
    connect(daemon_process_, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(daemon_error(QProcess::ProcessError)));
    connect(daemon_process_, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(daemon_finished(int,QProcess::ExitStatus)));

    timer_id_ = startTimer(1000); // 1s

    // ipfs should be in the PATH
    QStringList args;
    args << "daemon";
    args << "--init";
    daemon_process_->start(QString("ipfs"), args);

    state_ = LAUNCH_DAEMON;
}

void Ipfs::launch_access(IpfsAccess *access)
{
    access->reply = manager_->get(*access->request);

    connect(access->reply, &QNetworkReply::finished,
            this, [access]()
    {
        if(access->reply->error())
        {
            qDebug() << "http error: " << access->reply->errorString() << endl;
            qDebug() << "request: " << access->request->url() << endl;

            // Todo: relaunch ?
            return;
        }

        emit access->finished();
    });
}

Ipfs::~Ipfs()
{
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

void Ipfs::daemon_started()
{
    qDebug() << "daemon started";
}

void Ipfs::daemon_error(QProcess::ProcessError error)
{
    qDebug() << "daemon error " << error;
}

void Ipfs::daemon_finished(int exit_code, QProcess::ExitStatus exit_status)
{
    qDebug() << "daemon finished exit code: " << exit_code
             << "exit status: " << exit_status;
}

void Ipfs::timerEvent(QTimerEvent *)
{
    if(state_ == LAUNCH_DAEMON)
    {
        QByteArray stdout_ = daemon_process_->readAllStandardOutput();
        QRegExp regex = QRegExp("API server listening on \\/([^\\/]+)\\/([^\\/]+)\\/([^\\/]+)\\/([^\\/]+)\\n");

        int index = regex.indexIn(stdout_);
        if(index >= 0)
        {
            //QString network = regex.cap(1);
            api_ip_ = regex.cap(2);
            //QString transport = regex.cap(3);
            api_port_ = regex.cap(4);

            killTimer(timer_id_);
            state_ = RUNNING;
            init_commands();

            while(!access_buffer_.empty())
            {
                launch_access(access_buffer_.dequeue());
            }
        }
    }
}

IpfsAccess::~IpfsAccess()
{
    delete this->request;
    this->reply->deleteLater();
}

const QJsonObject IpfsAccess::json()
{
    QString str = this->reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    return doc.object();
}
