#ifndef IPFSLOG_H
#define IPFSLOG_H

#include <QObject>
#include <QString>

class IpfsLog : public QObject
{
    Q_OBJECT

public:
    enum IpfsLogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        PANIC
    };

public:
    explicit IpfsLog(QObject *parent = 0);
    virtual ~IpfsLog() {}

public:
    void log_level(const QString subsystem, IpfsLogLevel level);
    void log_level_all(IpfsLogLevel level);
};

#endif // IPFSLOG_H
