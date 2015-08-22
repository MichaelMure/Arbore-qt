#include "sharemodel.h"
#include "share.h"
#include "persist/persist.h"
#include "ipfs/ipfshash.h"

#include <QDebug>

ShareModel::ShareModel(QObject *parent) :
    QAbstractListModel(parent)
{
    foreach (Share *share, Persist::instance()->share.get_all())
    {
        addShare(share);
    }

    // FAKE DATA FOR NOW
    if(shares_.count() > 0 )
        return;

    Share *share = new Share(this);
    share->set_title("WebUI");
    share->add_hash(IpfsHash("QmXX7YRpU7nNBKfw75VG7Y1c3GwpSAGHRev67XVPgZFv9R"), Object::DIRECTORY);
    addShare(share);

    share = new Share(this);
    share->set_title("fake 2 with overly long name, like really long with a lot of letters and phrase and stuff");
    share->add_hash(IpfsHash("QmTkzDwWqPbnAh5YiV5VwcTLnGdwSNsNTn2aDxdXBFca7D"), Object::DIRECTORY);
    addShare(share);

    share = new Share(this);
    share->set_title("Example 3");
    share->add_hash(IpfsHash("QmX6gcmX2vy2gs5dWB45w8aUNynEiqGhLayXySGb7RF2TM"), Object::DIRECTORY);
    addShare(share);
}

ShareModel::~ShareModel()
{
    foreach (Share *share, shares_)
    {
        Persist::instance()->share.persist(share);
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

int ShareModel::count() const
{
    return shares_.size();
}

void ShareModel::addShare(QString hash)
{
    Share *share = new Share();
    share->set_title(hash);
    try
    {
        share->add_hash(IpfsHash(hash));
    }
    catch(...)
    {
        qDebug() << "Invalid hash, doing nothing";
        return;
    }

    addShare(share);
}

Share *ShareModel::getShare(int index)
{
    return shares_.at(index);
}

void ShareModel::addShare(Share *share)
{
    beginInsertRows(QModelIndex(), shares_.size(), shares_.size());
    shares_.append(share);
    endInsertRows();

    connect(share, &Share::dataChanged, [this, share]()
    {
        qDebug("datachanged");
        QModelIndex index = this->index(this->shares_.indexOf(share));
        emit dataChanged(index, index);
    });

    connect(share, &Share::shareChanged, [this, share]()
    {
        qDebug("sharechanged");
        QModelIndex index = this->index(this->shares_.indexOf(share));
        emit dataChanged(index, index);
    });
}
