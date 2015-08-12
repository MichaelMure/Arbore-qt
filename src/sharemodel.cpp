#include "sharemodel.h"
#include "share.h"
#include "persist/persist.h"
#include "ipfs/ipfshash.h"

ShareModel::ShareModel(QObject *parent) :
    QAbstractListModel(parent)
{
    shares_ = Persist::instance().share.get_all();

    // FAKE DATA FOR NOW
    if(shares_.count() > 0 )
        return;

    Share *share = new Share(this);
    share->set_title("WebUI");
    share->add_hash(IpfsHash("QmXX7YRpU7nNBKfw75VG7Y1c3GwpSAGHRev67XVPgZFv9R"), Object::DIRECTORY);
    shares_.append(share);

    share = new Share(this);
    share->set_title("fake 2 with overly long name, like really long with a lot of letters and phrase and stuff");
    share->add_hash(IpfsHash("QmTkzDwWqPbnAh5YiV5VwcTLnGdwSNsNTn2aDxdXBFca7D"), Object::DIRECTORY);
    shares_.append(share);

    share = new Share(this);
    share->set_title("Example 3");
    share->add_hash(IpfsHash("QmX6gcmX2vy2gs5dWB45w8aUNynEiqGhLayXySGb7RF2TM"), Object::DIRECTORY);
    shares_.append(share);
}

ShareModel::~ShareModel()
{
    foreach (Share *share, shares_)
    {
        Persist::instance().share.persist(share);
    }
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
        return dl->title();
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

Share *ShareModel::getShare(int index)
{
    return shares_.at(index);
}
