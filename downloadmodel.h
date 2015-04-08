#ifndef DOWNLOADMODEL_H
#define DOWNLOADMODEL_H

#include <QAbstractListModel>

class Download;

class DownloadModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DownloadModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QVector<Download*> downloads_;

signals:

public slots:

};

#endif // DOWNLOADMODEL_H

