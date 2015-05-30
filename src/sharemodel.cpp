#include "sharemodel.h"
#include "share.h"
#include "ipfs/ipfshash.h"

ShareModel::ShareModel(QObject *parent) :
    QAbstractListModel(parent)
{
    // FAKE DATA FOR NOW
    shares_.append(new Share("WebUI", IpfsHash("QmXX7YRpU7nNBKfw75VG7Y1c3GwpSAGHRev67XVPgZFv9R"), this));
    shares_.append(new Share("fake 2 with overly long name, like really long with a lot of letters and phrase and stuff",
                                   IpfsHash("QmTkzDwWqPbnAh5YiV5VwcTLnGdwSNsNTn2aDxdXBFca7D"), this));
    shares_.append(new Share("Example 3", IpfsHash("QmX6gcmX2vy2gs5dWB45w8aUNynEiqGhLayXySGb7RF2TM"), this));
}

QHash<int, QByteArray> ShareModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ProgressRole] = "progress";
    roles[SizeTotalRole] = "sizeTotal";
    roles[SizeLocalRole] = "sizeLocal";
    roles[BlockTotalRole] = "blockTotal";
    roles[BlockLocalRole] = "blockLocal";
    roles[FileTotalRole] = "fileTotal";
    roles[FileLocalRole] = "fileLocal";
    return roles;
}

int ShareModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return shares_.size();
}

QVariant ShareModel::data(const QModelIndex &index, int role) const
{
    const Share *dl = shares_.value(index.row(), 0);
    switch(role)
    {
    case NameRole:
        return dl->name();
    case ProgressRole:
        return dl->progress();
    case SizeTotalRole:
        return dl->size_total();
    case SizeLocalRole:
        return dl->size_local();
    case BlockTotalRole:
        return dl->block_total();
    case BlockLocalRole:
        return dl->block_local();
    case FileTotalRole:
        return dl->file_total();
    case FileLocalRole:
        return dl->file_local();
    }
    return QVariant();
}
