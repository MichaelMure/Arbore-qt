#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QString>

class Download : public QObject
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = 0);
    explicit Download(QString name, float progress, int size, QObject *parent = 0); // only here for fake data
    virtual ~Download() {}

    const QString& name() const;
    float progress() const;
    int size() const; // size in octet

private:
    // FAKE DATA FOR NOW
    QString name_;
    float progress_;
    int size_;
};

#endif // DOWNLOAD_H
