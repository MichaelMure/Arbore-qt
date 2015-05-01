#ifndef IPFSCOMMAND_H
#define IPFSCOMMAND_H

#include <QObject>

class QNetworkReply;

class IpfsCommand : public QObject
{
    Q_OBJECT
public:
    explicit IpfsCommand(QObject *parent = 0) : QObject(parent) {}
    virtual void init() = 0;
    virtual void on_reply(const QJsonObject *json) = 0;

protected:
    virtual ~IpfsCommand() {}
};

#endif // IPFSCOMMAND_H
