#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "object.h"
#include "ipfs/ipfshash.h"

#include <QHash>

class FileReply;

struct Child
{
    IpfsHash hash;
    QString name;
    uint size;
    Object *object;
};

class Directory : public Object
{
    Q_OBJECT
public:
    Directory(const IpfsHash &hash, const QString &name = "");
    Directory(const QString &hash, const QString &name = "");
    Directory(const FileReply *reply);
    virtual ~Directory();

private:
    Q_DISABLE_COPY(Directory)

    // Object interface
public:
    ObjectType type() const;
    uint size_total() const;
    uint size_local() const;
    uint block_total() const;
    uint block_local() const;
    uint file_total() const;
    uint file_local() const;
    bool metadata_local() const;

public:
    const QHash<QString, Child *>& getChilds() const;

private:
    void parse_file_reply(const FileReply *reply);

private:
    bool metadata_local_;
    QHash<IpfsHash, Child *> child_hashes_;
    QHash<QString, Child *> child_names_;
};

#endif // DIRECTORY_H
