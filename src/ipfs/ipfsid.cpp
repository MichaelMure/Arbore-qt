#include "ipfsid.h"
#include "ipfs.h"

#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>

const QString API_URL = "http://127.0.0.1:5001/api/v0/id";

IpfsId::IpfsId(QObject *parent)
    : AbstractIpfsCommand(parent),
      valid_data_(false),
      id_(NULL)
{
}

void IpfsId::init()
{
    Ipfs::instance().query(API_URL, this);
}

void IpfsId::on_reply(const QJsonObject *json)
{
    QJsonValue id_value = json->value("ID");

    if(id_)
        delete(id_);

    id_ = new IpfsHash(id_value.toString());

    valid_data_ = true;
    qDebug() << "Got local ID: " << id_->ToString();
}

bool IpfsId::valid_data() const
{
    return valid_data_;
}

const IpfsHash& IpfsId::id() const
{
    return *id_;
}




