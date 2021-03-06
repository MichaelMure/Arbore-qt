#ifndef SHAREMODEL_H
#define SHAREMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>

class Share;

class ShareModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ShareRoles {
        NameRole = Qt::UserRole,
        ProgressRole,
        StateRole,
        SizeTotalRole,
        SizeLocalRole,
        BlockTotalRole,
        BlockLocalRole,
        FileTotalRole,
        FileLocalRole
    };

    explicit ShareModel(QObject *parent = 0);
    virtual ~ShareModel();

private:
    Q_DISABLE_COPY(ShareModel)

public:
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE
    int count() const;

    Q_INVOKABLE
    void addShare(QString hash);

    Q_INVOKABLE
    Share* getShare(int index);

    Q_INVOKABLE
    void removeShare(int index);

private:
    void addShare(Share *share);

private:
    QVector<Share*> shares_;
};

#endif // SHAREMODEL_H

