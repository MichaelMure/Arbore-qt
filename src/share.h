#ifndef SHARE_H
#define SHARE_H

#include <QDateTime>
#include <QDir>
#include <QObject>
#include <QString>

#include "object.h"

class IpfsHash;

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
    // The repository can access the private member
    friend class ShareRepository;

    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY shareChanged)
    Q_PROPERTY(QString description READ description NOTIFY shareChanged)
    Q_PROPERTY(QDir path READ path NOTIFY shareChanged)
    Q_PROPERTY(QDateTime date_creation READ date_creation NOTIFY shareChanged)
    Q_PROPERTY(bool starred READ starred WRITE set_starred NOTIFY shareChanged)
    Q_PROPERTY(QString textual_arborescence READ textual_arborescence NOTIFY shareChanged)
    Q_PROPERTY(float progress READ progress NOTIFY dataChanged)

public:
    explicit Share(QObject *parent = 0);
    virtual ~Share() {}

    /**
     * @return the database Id of the Share
     */
    int id() const;

    /**
     * @return the title of the share, as defined by the creator
     */
    const QString& title() const;
    void set_title(const QString& title);

    /**
     * @return the description of the share, as defined by the creator
     */
    const QString description() const;

    /**
     * @return the path on disk where it is/will be located
     */
    const QDir& path() const;

    /**
     * @return that date of creation
     */
    const QDateTime& date_creation() const;

    /**
     * @return true if the share is starred by the user
     */
    bool starred() const;
    void set_starred(const bool &starred);

    /**
     * @return a textual representation of the file arborescence shared
     */
    QString textual_arborescence() const;

    /**
     * @return the current state of the share
     */
    ShareState state() const;

    /**
     * @return it's progress [0-->1]
     */
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

    /**
     * Construct a Share from only a hash.
     * @param hash
     */
    void add_hash(const IpfsHash &hash);

    /**
     * Construct a Share from a hash that we already know the type of object
     * @param hash
     * @param type
     */
    void add_hash(const IpfsHash &hash, Object::ObjectType type);

private:
    void set_description(const QString& description);
    void set_path(const QDir& path);
    void set_date_creation(const QDateTime &date);
    void set_state(const ShareState state);

signals:
    void shareChanged();
    void dataChanged();

private:
    int id_;
    QString title_;
    QString description_;
    QDir path_;
    QDateTime creation_date_;
    bool starred_;
    ShareState state_;
    QList<Object*> objects_;
};

#endif // SHARE_H
