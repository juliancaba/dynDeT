#include "BitstreamsWidget.h"
#include <QHeaderView>
#include <QCheckBox>

BitstreamsWidget::BitstreamsWidget(QWidget *parent, Design *des) : QWidget(parent), mainDesign(des) {
    createBitstreamsGroup();

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(bitstreamsGroup, 0, 0);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);
}

void BitstreamsWidget::createBitstreamsGroup() {
    QStringList tableHeader;

    bitstreamsGroup = new QGroupBox(tr("Listado de los nuevos bitstreams que se van a generar"));

    //Create the table of bitstreams
    tableHeader.append(tr("Nombre"));
    tableHeader.append(tr("Aplicar la compresión de Xilinx"));

    bitstreamsTable = new QTableWidget(bitstreamsGroup);
    bitstreamsTable->setColumnCount(2);
    bitstreamsTable->setColumnWidth(1, 200);
    bitstreamsTable->setHorizontalHeaderLabels(tableHeader);
    bitstreamsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    bitstreamsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    bitstreamsTable->verticalHeader()->hide();
    bitstreamsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bitstreamsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bitstreamsTable->setSelectionMode(QAbstractItemView::NoSelection);

    //Create the layout for the groupbox
    bitstreamsGroupLayout = new QGridLayout();
    bitstreamsGroupLayout->addWidget(bitstreamsTable, 0, 0);
    bitstreamsGroup->setLayout(bitstreamsGroupLayout);
}

void BitstreamsWidget::addBitstreams(QStringList &bitstreams) {
    int newRowIndex;
    QTableWidgetItem *auxName;
    QCheckBox *auxApplyCompression;

    bitstreamsTable->setRowCount(0);

    for (QStringList::size_type i = 0; i < bitstreams.size(); i++) {
        auxName = new QTableWidgetItem(bitstreams[i]);
        auxApplyCompression = new QCheckBox();
        auxApplyCompression->setChecked(false);

        newRowIndex = bitstreamsTable->rowCount();
        bitstreamsTable->insertRow(newRowIndex);
        bitstreamsTable->setItem(newRowIndex, 0, auxName);
        bitstreamsTable->setCellWidget(newRowIndex, 1, auxApplyCompression);
    }
}

void BitstreamsWidget::addToDesign() {
    QTableWidgetItem *name;
    QCheckBox *applyCompression;
    QString nameStr, configID;
    std::vector<Configuration> configs;
    int rows;
    bool cont;

    configs = mainDesign->getConfigurations();
    rows = bitstreamsTable->rowCount();

    for (int i = 0; i < rows; i++) {
        name = bitstreamsTable->item(i, 0);
        applyCompression = qobject_cast<QCheckBox *>(bitstreamsTable->cellWidget(i, 1));

        if (applyCompression->isChecked()) {
            nameStr = name->text();

            if (nameStr == "reference") {
                mainDesign->setReferenceXilinxCompression(true);
            }
            else {
                if (nameStr == "blanking") {
                    mainDesign->setBlankingXilinxCompression(true);
                }
                else {
                    //Find the configuration to obtain its ID
                    cont = true;
                    for (size_t i = 0; i < configs.size() && cont; i++) {
                        if (nameStr == QString::fromStdString(configs[i].getName())) {
                            configID = QString::fromStdString(configs[i].getConfigID());
                            cont = false;
                        }
                    }
                    mainDesign->setXilinxCompression(configID.toStdString(), true);
                }
            }
        }
    }
}

void BitstreamsWidget::enableAllEditElements() {
    int rows;
    QCheckBox *applyCompression;

    rows = bitstreamsTable->rowCount();
    for (int i = 0; i < rows; i++) {
        applyCompression = qobject_cast<QCheckBox *>(bitstreamsTable->cellWidget(i, 1));
        applyCompression->setEnabled(true);
    }
}

void BitstreamsWidget::disableAllEditElements() {
    int rows;
    QCheckBox *applyCompression;

    rows = bitstreamsTable->rowCount();
    for (int i = 0; i < rows; i++) {
        applyCompression = qobject_cast<QCheckBox *>(bitstreamsTable->cellWidget(i, 1));
        applyCompression->setEnabled(false);
    }
}
