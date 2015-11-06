#include "ipfsid.h"
#include "ipfs.h"

#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>

const QString API_COMMAND = "id";

IpfsId::IpfsId(QObject *parent)
    : QObject(parent),
      valid_data_(false),
      id_(NULL)
{
}

void IpfsId::init()
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND);
    IpfsAccess *access = Ipfs::instance()->query(url);

    connect(access, &IpfsAccess::finished,
            this, [this, access]()
    {
        const QJsonObject &json = access->json();

        QJsonValue id_value = json.value("ID");

        if(this->id_)
            delete(this->id_);

        this->id_ = new IpfsHash(id_value.toString());

        valid_data_ = true;
        qDebug() << "Got local ID: " << id_;

        delete access;
    });
}

bool IpfsId::valid_data() const
{
    return valid_data_;
}

const IpfsHash& IpfsId::id() const
{
    return *id_;
}




