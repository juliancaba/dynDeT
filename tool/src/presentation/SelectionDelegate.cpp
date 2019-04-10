#include "SelectionDelegate.h"

SelectionDelegate::SelectionDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void SelectionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QVariant background = index.data(Qt::BackgroundRole);

    if (background.canConvert<QBrush>()) {
        QPen itemPen;
        QVariant text = index.data(Qt::DisplayRole);

        itemPen = painter->pen();
        itemPen.setColor(QColor(0, 0, 0));
        itemPen.setWidth(1);
        painter->setPen(itemPen);

        //Fill the current item with its default background (QBrush)
        painter->setBrush(background.value<QBrush>());
        painter->drawRect(option.rect);

        //Draw the current text of the item
        if (text.canConvert<QString>()) {
            QString textStr = text.value<QString>();

            if (textStr != "") {
                QColor backgroundColor;
                QFont font;
                int r, g, b;

                //Get the background color
                backgroundColor = background.value<QBrush>().color();
                backgroundColor.getRgb(&r, &g, &b);

                font = painter->font();
                font.setBold(true);
                painter->setFont(font);

                if ((r + g + b)/3 > 128) {
                    painter->setPen(QColor(0, 0, 0));
                }
                else {
                    painter->setPen(QColor(255, 255, 255));
                }
            }

            painter->drawText(option.rect, Qt::AlignCenter, textStr);
        }

        //If the item is disabled, add another QBrush to the background
        if (!(option.state & QStyle::State_Enabled)) {
            QBrush brush(QColor(255, 0, 0, 75));
            painter->fillRect(option.rect, brush);
        }
        else {
            //If the item is selected, add another QBrush to the background (more transparent)
            if (option.state & QStyle::State_Selected) {
                QBrush brush(QColor(255, 255, 255, 75));
                painter->fillRect(option.rect, brush);
            }
        }
    }
}
