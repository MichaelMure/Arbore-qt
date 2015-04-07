#ifndef IPFS_H
#define IPFS_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class Ipfs : public QObject
{
    Q_OBJECT

public:
    explicit Ipfs(QObject *parent = 0);
    ~Ipfs();

    void test();

private slots:
    void replyFinished (QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;

};

#endif // IPFS_H
