#ifndef SHAREMODEL_H
#define SHAREMODEL_H

#include <QAbstractListModel>

class Share;

class ShareModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ShareModel(QObject *parent = 0);
    virtual ~ShareModel() {}

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QVector<Share*> shares_;

signals:

public slots:

};

#endif // SHAREMODEL_H

