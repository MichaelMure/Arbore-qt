#include "ipfs.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
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

void Ipfs::query(const QString &command, IApiListener *listener)
{
    query(api_url(command), listener);
}

void Ipfs::query(const QUrl &url, IApiListener *listener)
{
//    qDebug() << "HTTP query: " << url;
    QNetworkRequest request = QNetworkRequest(url);

    QNetworkReply *reply = manager_->get(request);

    if(listener)
    {
        replies_listener[reply] = listener;
    }

    connect(reply, SIGNAL(finished()),
            this, SLOT(replyFinished()));
}

QNetworkReply *Ipfs::manual_query(const QString &command)
{
    return manual_query(api_url(command));
}

QNetworkReply *Ipfs::manual_query(const QUrl &url)
{
//    qDebug() << "HTTP query: " << url;
    QNetworkRequest request = QNetworkRequest(url);

    return manager_->get(request);
}

void Ipfs::init()
{
    manager_ = new QNetworkAccessManager();
    daemon_process_ = NULL;

    // Ping the HTTP API to find out if a daemon is running
    state_ = PING_DAEMON;
    query("version", NULL);
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

void Ipfs::replyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

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
        return;
    }

    if(reply->error())
    {
        qDebug() << "http error: " << reply->errorString();
        qDebug() << "request: " << reply->request().url();
        return;
    }

    QString str = reply->readAll();
    //qDebug() << "http reply : " << str;

    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject json = doc.object();

    if(replies_listener.contains(reply))
    {
        replies_listener[reply]->on_reply(&json);
        replies_listener.remove(reply);
    }

    reply->deleteLater();
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
        }
    }
}
