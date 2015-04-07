#include "ipfs.h"
#include <QJsonDocument>
#include <QJsonObject>



Ipfs::Ipfs(QObject *parent)
 : QObject(parent)
{
    manager = new QNetworkAccessManager();

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

Ipfs::~Ipfs()
{
    delete manager;
}

void Ipfs::test()
{
    QUrl url("http://127.0.0.1:5001/api/v0/version/");

    manager->get(QNetworkRequest(url));
}

void Ipfs::replyFinished(QNetworkReply *reply)
{
    QString data = (QString) reply->readAll();

    QJsonDocument doc(QJsonDocument::fromJson(data.toUtf8()));
    QJsonObject json = doc.object();

    qDebug() << json["Version"].toString();
}
