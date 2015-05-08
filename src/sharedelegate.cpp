#include "sharedelegate.h"
#include "share.h"

#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QStyle>

enum
{
    GUI_PAD = 6
};

ShareDelegate::ShareDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void ShareDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const Share * dl (index.data(Qt::UserRole).value<const Share*>());
    QStyle *style = option.widget ? option.widget->style() : QApplication::style();

    painter->save();
    painter->setClipRect (option.rect);

    const bool isItemSelected ((option.state & QStyle::State_Selected) != 0);
    const bool isItemEnabled ((option.state & QStyle::State_Enabled) != 0);
    const bool isItemActive ((option.state & QStyle::State_Active) != 0);

    QPalette::ColorGroup cg = isItemEnabled || !isItemActive ? // TODO: download paused
                QPalette::Normal :
                QPalette::Disabled;

    QPalette::ColorRole cr = isItemSelected ?
                QPalette::HighlightedText :
                QPalette::Text;

    painter->setPen(option.palette.color(cg, cr));

    // Fill background if selected
    if(isItemSelected)
    {
        painter->fillRect(option.rect, option.palette.brush(cg, QPalette::Highlight));
    }

    // Remove a margin from the full rect
    const QRect contentRect (option.rect.adjusted(4, 4, -4, -4));

    // Icon
    const int ICON_SIZE = 40;
    const QRect iconRect = style->alignedRect(option.direction,
                                        Qt::AlignLeft | Qt::AlignVCenter,
                                        QSize(ICON_SIZE, ICON_SIZE),
                                        contentRect);
    painter->fillRect(iconRect, Qt::red);

    // Name
    QFont nameFont = option.font;
    nameFont.setWeight(QFont::Bold);
    const QFontMetrics nameFM (nameFont);
    QString nameText = dl->name();
    const QSize nameSize (nameFM.size(Qt::TextSingleLine, nameText));
    const QRect nameRect = contentRect.adjusted(ICON_SIZE + GUI_PAD, 0, 0, nameSize.height());
    nameText = nameFM.elidedText(nameText, Qt::ElideRight, nameRect.width());

    painter->setFont(nameFont);
    painter->drawText(nameRect, nameText);


    painter->restore ();
}

QSize ShareDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(50);
    return result;
}
