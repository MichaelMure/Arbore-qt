#include "ipfsadd.h"
#include "ipfs/ipfs.h"

#include <QUrl>
#include <QUrlQuery>
#include <QTextStream>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QDebug>
#include <QNetworkReply>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QException>
#include <QJsonObject>

const QString API_COMMAND = "add";

IpfsAdd::IpfsAdd(QObject *parent)
    : QObject(parent)
{
}

HashReply *IpfsAdd::add(QString filepath)
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND);

    QFileInfo info = QFileInfo(filepath);
    if(!info.exists())
    {
        throw new std::runtime_error("Path does not exist on disk.");
    }

    if(!info.isFile())
    {
        throw new std::runtime_error("Path is not a file.");
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    part.setHeader(QNetworkRequest::ContentDispositionHeader,
        QVariant(QString("file; filename=\"%1\"").arg(info.fileName())) // todo: relative filename
    );

    QFile *file = new QFile(info.absoluteFilePath());
    bool ok = file->open(QIODevice::ReadOnly);
    if(!ok)
    {
        throw new std::runtime_error("Could not open the file.");
    }

    part.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(part);

    HashReply *add_reply = new HashReply();
    IpfsAccess *access = Ipfs::instance()->query(url, multiPart);

    connect(access, &IpfsAccess::finished,
            this, [access, add_reply]()
    {
        const QJsonObject &json = access->json();
        add_reply->hash = IpfsHash(json.value("Hash").toString());

        emit add_reply->finished();

        delete access;
    });

    return add_reply;
}
