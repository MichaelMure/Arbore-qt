#include "ipfsversion.h"
#include "ipfs/ipfs.h"
#include <QDebug>
#include <QUrl>
#include <QJsonObject>

IpfsVersion::IpfsVersion(QObject *parent) :
    IpfsCommand(parent)
{
}

int IpfsVersion::major() const
{
    return major_;
}

int IpfsVersion::minor() const
{
    return minor_;
}

int IpfsVersion::micro() const
{
    return micro_;
}

void IpfsVersion::init()
{
    QUrl url("http://127.0.0.1:5001/api/v0/version/");
    Ipfs::instance().query(url, this);
}

void IpfsVersion::on_reply(const QJsonObject *json)
{
    QJsonValue value = json->value("Version");
    QString valueStr = value.toString();
    QStringList numbers = valueStr.split(".");

    major_ = numbers[0].toInt();
    minor_ = numbers[1].toInt();
    micro_ = numbers[2].toInt();

    qDebug() << "Got version: " << this->ToString();
}

QString IpfsVersion::ToString() const
{
    QString str;
    str += QString::number(major()) + ".";
    str += QString::number(minor()) + ".";
    str += QString::number(micro());

    return str;
}

