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
        NameRole = Qt::UserRole + 1,
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
    virtual ~ShareModel() {}


    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QVector<Share*> shares_;
};

#endif // SHAREMODEL_H

