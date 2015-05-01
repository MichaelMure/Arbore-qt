#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QString>

enum DownloadState {
    DOWNLOADING,
    PAUSED,
    FINISHED,
};

class Download : public QObject
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = 0);
    // only here for fake data
    explicit Download(QString name, DownloadState state, float progress, int size, QObject *parent = 0);
    virtual ~Download() {}

    const QString& name() const;
    DownloadState state() const;
    float progress() const;
    int size() const; // size in octet

private:
    // FAKE DATA FOR NOW
    QString name_;
    DownloadState state_;
    float progress_;
    int size_;
};
Q_DECLARE_METATYPE(const Download*)

#endif // DOWNLOAD_H
