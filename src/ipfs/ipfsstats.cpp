#include "ipfsstats.h"
#include "ipfs.h"

#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QDebug>

const QString API_COMMAND = "stats";

IpfsStats::IpfsStats(QObject *parent)
    : QObject(parent),
      valid_data_(false),
      total_in_(0),
      total_out_(0),
      rate_in_(0.0),
      rate_out_(0.0)
{
    startTimer(1000); // 1s
}

void IpfsStats::refresh()
{
    Ipfs::instance().query(API_COMMAND + "/bw", this);
}

QString IpfsStats::format_rate(uint rate)
{
    double val = (double) rate;
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("B");

    while(val >= 1024.0 && i.hasNext())
     {
        unit = i.next();
        val /= 1024.0;
    }
    return QString().setNum(val,'f',0)+" "+unit+"/s";
}

bool IpfsStats::valid_data() const
{
    return valid_data_;
}

uint IpfsStats::total_in() const
{
    return total_in_;
}

uint IpfsStats::total_out() const
{
    return total_out_;
}

float IpfsStats::rate_in() const
{
    return rate_in_;
}

float IpfsStats::rate_out() const
{
    return rate_out_;
}

const QString IpfsStats::ToString() const
{
    return QString("Down: %1  Up: %2")
            .arg(format_rate(rate_in_))
            .arg(format_rate(rate_out_));
}

void IpfsStats::init()
{
    refresh();
}

void IpfsStats::timerEvent(QTimerEvent *)
{
    refresh();
}

void IpfsStats::on_reply(const QJsonObject *json)
{
    total_in_ = json->value("TotalIn").toDouble();
    total_out_ = json->value("TotalOut").toDouble();
    rate_in_ = json->value("RateIn").toDouble();
    rate_out_ = json->value("RateOut").toDouble();
}

