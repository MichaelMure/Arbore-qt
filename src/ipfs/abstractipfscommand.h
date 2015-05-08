#ifndef IPFSCOMMAND_H
#define IPFSCOMMAND_H

#include <QObject>

class QNetworkReply;

/**
 * Base class for an IPFS command.
 * Define the interface used by the Ipfs class to init and relay
 * HTTP response from the IPFS HTTP API.
 */
class AbstractIpfsCommand : public QObject
{
    Q_OBJECT
public:
    explicit AbstractIpfsCommand(QObject *parent = 0) : QObject(parent) {}
    virtual void init() = 0;
    virtual void on_reply(const QJsonObject *json) = 0;

protected:
    virtual ~AbstractIpfsCommand() {}
};

#endif // IPFSCOMMAND_H
