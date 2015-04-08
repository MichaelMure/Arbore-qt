#include "downloadmodel.h"
#include "download.h"

DownloadModel::DownloadModel(QObject *parent) :
    QAbstractListModel(parent)
{
    // FAKE DATA FOR NOW
    downloads_.append(new Download("fake 1", 0.25f, 4568, this));
    downloads_.append(new Download("fake 2", 0.43f, 16531, this));
    downloads_.append(new Download("fake 3", 0.91f, 126533, this));
}

int DownloadModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return downloads_.size();
}

QVariant DownloadModel::data(const QModelIndex &index, int role) const
{
    // Check that the index is valid and within the correct range first:
    if (!index.isValid())
        return QVariant();
    if (index.row() >= downloads_.size())
        return QVariant();

    if(role == Qt::DisplayRole)
        return QVariant(downloads_.at(index.row())->name());

    return QVariant();
}
