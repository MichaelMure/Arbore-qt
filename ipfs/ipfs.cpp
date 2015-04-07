#include "ipfs.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

static bool initialized = false;

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

void Ipfs::query(QUrl &url, IpfsCommand *originator)
{
    QNetworkRequest request = QNetworkRequest(url);
    request.setOriginatingObject(originator);
    manager->get(request);
}

void Ipfs::init()
{
    manager = new QNetworkAccessManager();

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    version.init();
}

Ipfs::~Ipfs()
{
    manager->deleteLater();
}

void Ipfs::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "http error: " << reply->errorString() << endl;
        qDebug() << reply->readAll();
        return;
    }

    QString str = reply->readAll();
    qDebug() << "http reply : " << str;

    QJsonDocument doc(QJsonDocument::fromJson(str.toUtf8()));
    QJsonObject json = doc.object();

    IpfsCommand *command = (IpfsCommand*) reply->request().originatingObject();
    command->on_reply(&json);

    reply->deleteLater();
}
