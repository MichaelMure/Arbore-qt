#include "sharemodel.h"
#include "share.h"
#include "ipfs/ipfshash.h"

ShareModel::ShareModel(QObject *parent) :
    QAbstractListModel(parent)
{
    // FAKE DATA FOR NOW
    shares_.append(new Share("WebUI", IpfsHash("QmXX7YRpU7nNBKfw75VG7Y1c3GwpSAGHRev67XVPgZFv9R"), this));
    /*shares_.append(new Share("fake 2 with overly long name, like really long with a lot of letters and phrase and stuff",
                                   DOWNLOADING, 0.43f, 16531, this));
    shares_.append(new Share("fake 3", DOWNLOADING, 0.91f, 126533, this));*/
}

int ShareModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return shares_.size();
}

QVariant ShareModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    // Check that the index is valid and within the correct range first:
    if (!index.isValid())
        return var;
    if (index.row() >= shares_.size())
        return var;

    const Share *dl = shares_.value(index.row(), 0);
    switch(role)
    {
    case Qt::UserRole: // complex gui display
        var.setValue(dl); break;
    case Qt::DisplayRole: // text display
        var.setValue(dl->name()); break;
    }

    return var;
}
