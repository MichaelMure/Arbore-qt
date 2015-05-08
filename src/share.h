#ifndef SHARE_H
#define SHARE_H

#include <QObject>
#include <QString>

enum ShareState {
    DOWNLOADING,
    PAUSED,
    FINISHED,
};

class Share : public QObject
{
    Q_OBJECT
public:
    explicit Share(QObject *parent = 0);
    // only here for fake data
    explicit Share(QString name, ShareState state, float progress, int size, QObject *parent = 0);
    virtual ~Share() {}

    const QString& name() const;
    ShareState state() const;
    float progress() const;
    int size() const; // size in octet

private:
    // FAKE DATA FOR NOW
    QString name_;
    ShareState state_;
    float progress_;
    int size_;
};
Q_DECLARE_METATYPE(const Share*)

#endif // SHARE_H
