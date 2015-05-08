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

static bool initialized = false;

Ipfs::Ipfs()
    : state_(PING_DAEMON),
      refreshTimer_(this),
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

void Ipfs::query(const QString &command, AbstractIpfsCommand *originator)
{
    query(api_url(command), originator);
}

void Ipfs::query(const QUrl &url, AbstractIpfsCommand *originator)
{
    qDebug() << "HTTP query: " << url << endl;
    QNetworkRequest request = QNetworkRequest(url);
    request.setOriginatingObject(originator);
    manager_->get(request);
}

void Ipfs::init()
{
    manager_ = new QNetworkAccessManager();
    daemon_process_ = NULL;

    connect(manager_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    // Ping the HTTP API to find out if a daemon is running
    state_ = PING_DAEMON;
    query("version", NULL);
}

void Ipfs::init_commands()
{
    get.init();
    id.init();
    pin.init();
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

    connect(&refreshTimer_, SIGNAL(timeout()),
            this, SLOT(timer()));
    refreshTimer_.start(1000); // 1s

    // ipfs should be in the PATH
    QStringList args;
    args << "daemon";
    args << "--init";
    daemon_process_->start(QString("ipfs"), args);

    state_ = LAUNCH_DAEMON;
}

Ipfs::~Ipfs()
{
    daemon_process_->terminate();
    if(!daemon_process_->waitForFinished())
    {
        daemon_process_->kill();
    }
    manager_->deleteLater();
}

void Ipfs::replyFinished(QNetworkReply *reply)
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
        return;
    }

    if(reply->error())
    {
        qDebug() << "http error: " << reply->errorString() << endl;
        qDebug() << "request: " << reply->request().url() << endl;
        return;
    }

    QString str = reply->readAll();
    qDebug() << "http reply : " << str;

    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject json = doc.object();

    AbstractIpfsCommand *command = (AbstractIpfsCommand*) reply->request().originatingObject();
    command->on_reply(&json);

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

void Ipfs::timer()
{
    if(state_ == LAUNCH_DAEMON)
    {
        QString stdout = daemon_process_->readAllStandardOutput();
        QRegExp regex = QRegExp("API server listening on \\/([^\\/]+)\\/([^\\/]+)\\/([^\\/]+)\\/([^\\/]+)\\n");

        int index = regex.indexIn(stdout);
        if(index >= 0)
        {
            //QString network = regex.cap(1);
            api_ip_ = regex.cap(2);
            //QString transport = regex.cap(3);
            api_port_ = regex.cap(4);

            refreshTimer_.stop();
            state_ = RUNNING;
            init_commands();
        }
    }
}
