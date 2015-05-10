#ifndef IPFSSTATS_H
#define IPFSSTATS_H

#include <QObject>
#include "ipfs/iapilistener.h"
#include <QTimer>

class IpfsStats : public QObject, public IApiListener
{
    Q_OBJECT
public:
    explicit IpfsStats(QObject *parent = 0);

    uint total_in() const;
    uint total_out() const;
    float rate_in() const;
    float rate_out() const;
    const QString ToString() const;

public:
    void init();
    void on_reply(const QJsonObject *json);
    bool valid_data() const;

private slots:
    void refresh();

private:
    QTimer refreshTimer_;
    bool valid_data_;
    uint total_in_;
    uint total_out_;
    float rate_in_;
    float rate_out_;

    static QString format_rate(uint rate);
};

#endif // IPFSSTATS_H
