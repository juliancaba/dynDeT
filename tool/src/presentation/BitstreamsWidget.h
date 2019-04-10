#ifndef BITSTREAMSWIDGET_H
#define BITSTREAMSWIDGET_H

#include "Design.h"
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QTableWidget>

class BitstreamsWidget : public QWidget {
    Q_OBJECT
public:
    explicit BitstreamsWidget(QWidget *parent = 0, Design *des = 0);

    void addBitstreams(QStringList &bitstreams);
    void addToDesign();
    void enableAllEditElements();
    void disableAllEditElements();

private:
    Design *mainDesign;

    QGridLayout *layout;
    QGridLayout *bitstreamsGroupLayout;
    QGroupBox *bitstreamsGroup;
    QTableWidget *bitstreamsTable;

    void createBitstreamsGroup();
};

#endif
