#include "ipfslog.h"
#include "ipfs/ipfs.h"

#include <QUrl>
#include <QUrlQuery>

const QString API_COMMAND = "log";

IpfsLog::IpfsLog(QObject *parent)
    : QObject(parent)
{    
}

void IpfsLog::log_level(const QString subsystem, IpfsLog::IpfsLogLevel level)
{
    QUrl url = Ipfs::instance()->api_url(API_COMMAND + "/level");
    QUrlQuery query;

    QString level_str;
    switch(level)
    {
    case DEBUG:     level_str = "debug";    break;
    case INFO:      level_str = "info";     break;
    case WARNING:   level_str = "warning";  break;
    case ERROR:     level_str = "error";    break;
    case FATAL:     level_str = "fatal";    break;
    case PANIC:     level_str = "panic";    break;
    }

    query.addQueryItem("subsystem", subsystem);
    query.addQueryItem("level", level_str);

    url.setQuery(query);
    Ipfs::instance()->query(url);
}

void IpfsLog::log_level_all(IpfsLog::IpfsLogLevel level)
{
    log_level("all", level);
}

