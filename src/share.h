#ifndef SHARE_H
#define SHARE_H

#include <QObject>
#include <QString>

class IpfsHash;
class Object;

enum ShareState {
    UNITIALIZED,
    DOWNLOADING,
    PAUSED,
    FINISHED,
};

/**
 * This class define a share (download or upload), ie, a collection of directories
 * and files that may be available somewhere.
 */
class Share : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(float progress READ progress NOTIFY dataChanged)

public:
    explicit Share(QObject *parent = 0);
    explicit Share(QString name, const IpfsHash &hash, QObject *parent = 0);
    virtual ~Share() {}

    const QString& name() const;

    ShareState state() const;

    float progress() const;

    /**
     * @return the total size in octet of the object
     */
    uint size_total() const;

    /**
     * @return the size of the data that is available locally
     */
    uint size_local() const;

    /**
     * @return the total number of block of the object
     */
    uint block_total() const;

    /**
     * @return the number of block of the data available locally
     */
    uint block_local() const;

    /**
     * @return the total number of file in the object
     */
    uint file_total() const;

    /**
     * @return the number of files available locally
     */
    uint file_local() const;

signals:
    void dataChanged();

private:
    QString name_;
    ShareState state_;
    QList<Object*> objects_;
};

#endif // SHARE_H
