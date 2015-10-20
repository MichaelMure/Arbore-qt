#include "sharefilter.h"
#include "sharemodel.h"
#include "share.h"

ShareFilter::ShareFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

ShareFilter::~ShareFilter()
{
}

bool ShareFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex childIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    QString shareName = childIndex.model()->data(childIndex, ShareModel::NameRole).value<QString>();
    Share::ShareState state = childIndex.model()->data(childIndex, ShareModel::StateRole).value<Share::ShareState>();

    QRegExp regExp = filterRegExp();
    regExp.setCaseSensitivity(Qt::CaseInsensitive);

    return shareName.contains(regExp) && (state != Share::ShareState::CREATING);
}

Share *ShareFilter::getShare(int index)
{
    ShareModel* model = (ShareModel*)sourceModel();
    return model->getShare(index);
}

void ShareFilter::addShare(QString hash)
{
    ShareModel* model = (ShareModel*)sourceModel();
    return model->addShare(hash);
}

int ShareFilter::count() const
{
    ShareModel* model = (ShareModel*)sourceModel();
    return model->count();
}
