#ifndef SHAREFILTER_H
#define SHAREFILTER_H

#include <QSortFilterProxyModel>
#include <QMetaType>
#include <QVariant>

class Share;

class ShareFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ShareFilter(QObject *parent = 0);
    virtual ~ShareFilter();

    Q_INVOKABLE
    Share* getShare(int);

    Q_INVOKABLE
    void addShare(QString hash);

    Q_INVOKABLE
    int count() const;

protected:
    virtual bool filterAcceptsRow(int, const QModelIndex&) const;
};

#endif // SHAREFILTER_H
