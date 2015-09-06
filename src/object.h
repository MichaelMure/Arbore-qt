#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>
#include "ipfs/ipfshash.h"

/**
 * This class define an object (file/directory) in a Ipfs Dag.
 */
class Object : public QObject
{
    Q_OBJECT
public:
    enum ObjectType
    {
        INVALID = -1,
        RAW = 0,
        DIRECTORY = 1,
        FILE = 2,
        METADATA = 3
    };
    static ObjectType decode_type(const QString &str);

    Object(const IpfsHash &hash, const QString &name = "");
    Object(const QString &hash, const QString &name = "");
    virtual ~Object() {}

private:
    Q_DISABLE_COPY(Object)

public:
    /**
     * @return the name of the object available, empty string otherwise.
     */
    const QString& name() const;

    /**
     * @return the hash of the object
     */
    const IpfsHash& hash() const;

    /**
     * @return the type of the object
     */
    virtual ObjectType type() const = 0;

    /**
     * @return the total size in octet of the object
     */
    virtual uint size_total() const = 0;

    /**
     * @return the size of the data that is available locally
     */
    virtual uint size_local() const = 0;

    /**
     * @return the total number of block of the object
     */
    virtual uint block_total() const = 0;

    /**
     * @return the number of block of the data available locally
     */
    virtual uint block_local() const = 0;

    /**
     * @return the total number of file in the object
     */
    virtual uint file_total() const = 0;

    /**
     * @return the number of files available locally
     */
    virtual uint file_local() const = 0;

    /**
     * @return true if all metadata (size, childs, ..) are known
     */
    virtual bool metadata_local() const = 0;

signals:
    /**
     * @brief Notify that the local disponibility of this object changed.
     */
    void localityChanged();


protected:
    const QString name_;
    const IpfsHash hash_;
};

#endif // OBJECT_H
