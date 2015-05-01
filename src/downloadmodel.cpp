#include "downloadmodel.h"
#include "download.h"

DownloadModel::DownloadModel(QObject *parent) :
    QAbstractListModel(parent)
{
    // FAKE DATA FOR NOW
    downloads_.append(new Download("fake 1", DOWNLOADING, 0.25f, 4568, this));
    downloads_.append(new Download("fake 2 with overly long name, like really long with a lot of letters and phrase and stuff",
                                   DOWNLOADING, 0.43f, 16531, this));
    downloads_.append(new Download("fake 3", DOWNLOADING, 0.91f, 126533, this));
}

int DownloadModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return downloads_.size();
}

QVariant DownloadModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    // Check that the index is valid and within the correct range first:
    if (!index.isValid())
        return var;
    if (index.row() >= downloads_.size())
        return var;

    const Download *dl = downloads_.value(index.row(), 0);
    switch(role)
    {
    case Qt::UserRole: // complex gui display
        var.setValue(dl); break;
    case Qt::DisplayRole: // text display
        var.setValue(dl->name()); break;
    }

    return var;
}
