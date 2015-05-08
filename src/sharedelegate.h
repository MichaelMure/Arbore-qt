#ifndef SHAREDELEGATE_H
#define SHAREDELEGATE_H

#include <QStyledItemDelegate>

class ShareDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ShareDelegate(QObject *parent = 0);
    virtual ~ShareDelegate() {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // SHAREDELEGATE_H
