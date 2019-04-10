#ifndef SELECTIONDELEGATE_H
#define SELECTIONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class SelectionDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    SelectionDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif

