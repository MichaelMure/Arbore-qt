#ifndef IAPILISTENER_H
#define IAPILISTENER_H

class QJsonObject;

class IApiListener
{
public:
    virtual ~IApiListener() {}

    virtual void on_reply(const QJsonObject *json) = 0;
};

#endif // IAPILISTENER_H
