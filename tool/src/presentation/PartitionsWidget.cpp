#include "PartitionsWidget.h"
#include "MainWindow.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>

PartitionsWidget::PartitionsWidget(MainWindow *parent, Design *des) : QWidget(parent), mainDesign(des), mainWindowParent(parent), fpWidget(this) {
    createAddPartitionGroup();
    createPartitionsGroup();

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(addPartitionGroup, 0, 0);
    layout->addWidget(partitionsGroup, 1, 0);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);
}

void PartitionsWidget::enableAllEditElements() {
    addPartitionGroup->setEnabled(true);
    partitionsGroup->setEnabled(true);
}

void PartitionsWidget::disableAllEditElements() {
    addPartitionGroup->setEnabled(false);
    //partitionsGroup->setEnabled(false);
}

QStringList PartitionsWidget::getPartitionsIDs() {
    QTableWidgetItem *idItem;
    QStringList result;

    for (int i = 0; i < partitionsTable->rowCount(); i++) {
        idItem = partitionsTable->item(i, 0);
        result.append(idItem->text());
    }

    return result;
}

void PartitionsWidget::loadFromDesign() {
    int newRowIndex;
    //QTableWidgetItem *idWItem, *nameWItem, *instanceWItem, *srcWItem, *resetAfterReconfigWItem, *snappingModeWItem, *hwResourcesWItem;
    QTableWidgetItem *nameWItem, *instanceWItem, *srcWItem, *resetAfterReconfigWItem, *snappingModeWItem, *hwResourcesWItem;
    QString hwResourcesStr, resetAfterReconfigStr, snappingModeStr;
    std::vector<std::string> hwResources;
    std::vector<ReconfigPartition> partitions;

    partitions = mainDesign->getPartitions();

    for (size_t i = 0; i < partitions.size(); i++) {
        //Get the data of the partition
        //idWItem = new QTableWidgetItem(QString::fromStdString(partitions[i].getPartitionID()));
        nameWItem = new QTableWidgetItem(QString::fromStdString(partitions[i].getName()));
        instanceWItem = new QTableWidgetItem(QString::fromStdString(partitions[i].getInstanceName()));
        srcWItem = new QTableWidgetItem(QString::fromStdString(partitions[i].getSrcDir()));

        hwResources = partitions[i].getHwResources();
        for (size_t j = 0; j < hwResources.size(); j++) {
            hwResourcesStr += QString::fromStdString(hwResources[j]) + " ";
        }
        hwResourcesWItem = new QTableWidgetItem(hwResourcesStr);

        resetAfterReconfigStr = partitions[i].getResetAfterReconfig() ? "true" : "false";
        snappingModeStr = partitions[i].getSnappingMode() ? "true" : "false";
        resetAfterReconfigWItem = new QTableWidgetItem(resetAfterReconfigStr);
        snappingModeWItem = new QTableWidgetItem(snappingModeStr);

        //Add a new partition to the table
        newRowIndex = partitionsTable->rowCount();
        partitionsTable->insertRow(newRowIndex);
        //partitionsTable->setItem(newRowIndex, 0, idWItem);
        /*partitionsTable->setItem(newRowIndex, 1, nameWItem);
        partitionsTable->setItem(newRowIndex, 2, instanceWItem);
        partitionsTable->setItem(newRowIndex, 3, srcWItem);
        partitionsTable->setItem(newRowIndex, 4, resetAfterReconfigWItem);
        partitionsTable->setItem(newRowIndex, 5, snappingModeWItem);
        partitionsTable->setItem(newRowIndex, 6, hwResourcesWItem);
	*/
	
        partitionsTable->setItem(newRowIndex, 0, nameWItem);
        partitionsTable->setItem(newRowIndex, 1, instanceWItem);
        partitionsTable->setItem(newRowIndex, 2, srcWItem);
        partitionsTable->setItem(newRowIndex, 3, resetAfterReconfigWItem);
        partitionsTable->setItem(newRowIndex, 4, snappingModeWItem);
        partitionsTable->setItem(newRowIndex, 5, hwResourcesWItem);

	
        hwResourcesStr.clear();
    }
}

void PartitionsWidget::addAllPartitionsToDesign() {
    for (int i = 0; i < partitionsTable->rowCount(); i++) {
        ReconfigPartition partition;
        QString id, name, instance, src;
        QStringList hwResourcesSplited;
        bool resetAfterReconfig, snappingMode;

        //Get the data of the partition
        //id = partitionsTable->item(i, 0)->text();
        name = partitionsTable->item(i, 0)->text();
        instance = partitionsTable->item(i, 1)->text();
        src = partitionsTable->item(i, 2)->text();
        if (partitionsTable->item(i, 3)->text() == "true") {
            resetAfterReconfig = true;
        }
        else {
            resetAfterReconfig = false;
        }
        if (partitionsTable->item(i, 4)->text() == "true") {
            snappingMode = true;
        }
        else {
            snappingMode = false;
        }
        hwResourcesSplited = partitionsTable->item(i, 5)->text().split(" ");

        //partition = ReconfigPartition(id.toStdString(), name.toStdString(), instance.toStdString(), src.toStdString(), resetAfterReconfig, snappingMode);
	partition = ReconfigPartition(name.toStdString(), name.toStdString(), instance.toStdString(), src.toStdString(), resetAfterReconfig, snappingMode);
        for (QStringList::size_type i = 0; i < hwResourcesSplited.size(); i++) {
            partition.addHwResource(hwResourcesSplited[i].toStdString());
        }

        mainDesign->addPartition(partition);
    }
}

bool PartitionsWidget::check() {
    bool empty = false;

    if (partitionsTable->rowCount() == 0) {
        partitionsTable->setStyleSheet("background: #ffcccc");
        empty = true;
    }
    else {
        partitionsTable->setStyleSheet("");
    }

    return !empty;
}

void PartitionsWidget::enablePartitionsContextMenu() {
    connect(partitionsTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPartitionsContextMenu(QPoint)));
}

void PartitionsWidget::disablePartitionsContextMenu() {
    disconnect(partitionsTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPartitionsContextMenu(QPoint)));
}


void PartitionsWidget::setFlagsPartitions(QFlags<Qt::ItemFlag> flags) {
    for (int i = 0; i < partitionsTable->rowCount(); i++) {
        for (int j = 0; j < partitionsTable->columnCount(); j++) {
            partitionsTable->item(i, j)->setFlags(flags);
        }
    }
}

void PartitionsWidget::disableExistingPartitions() {
    setFlagsPartitions(Qt::ItemIsEnabled);
}

void PartitionsWidget::enableExistingPartitions() {
  setFlagsPartitions((Qt::ItemIsEnabled | Qt::ItemIsSelectable));
}

bool PartitionsWidget::checkLineEditEmpty(QLineEdit *le) {
    bool result = false;

    if (le->text().isEmpty()) {
        le->setStyleSheet("background: #ffcccc");
        result = true;
    }
    else {
        le->setStyleSheet("");
    }

    return result;
}

bool PartitionsWidget::checkPartitionIDOK() {
    bool result = true;
    QString partitionID;

    //partitionID = idLineEdit->text();

    partitionID = partiNameLineEdit->text();
    for (int i = 0; i < partitionsTable->rowCount() && result; i++) {
        if (partitionsTable->item(i, 0)->text() == partitionID) {
            result = false;
        }
    }

    if (result) {
      //idLineEdit->setStyleSheet("");
      partiNameLineEdit->setStyleSheet("");
    }
    else {
      //idLineEdit->setStyleSheet("background: #ffcccc");
	partiNameLineEdit->setStyleSheet("background: #ffcccc");
    }

    return result;
}


void PartitionsWidget::createAddPartitionGroup() {
    QString resetAfterReconfigTooltip, snappingModeTooltip;
    addPartitionGroup = new QGroupBox(tr("Añadir una nueva partición"));

    //idLabel = new QLabel(tr("ID:"));
    partiNameLabel = new QLabel(tr("Nombre del módulo reconfigurable:"));
    partiInstanceLabel = new QLabel(tr("Nombre de la instancia:"));
    sourceDirLabel = new QLabel(tr("Path a la instancia (Vivado Tree):"));
    partitionPropertiesLabel = new QLabel(tr("Propiedades:"));

    //idLineEdit = new QLineEdit();
    partiNameLineEdit = new QLineEdit();
    partiInstanceLineEdit = new QLineEdit();
    sourceDirLineEdit = new QLineEdit();
    addPartitionButton = new QPushButton(tr("Añadir partición"), addPartitionGroup);
    addPartitionButton->setFixedHeight(30);
    addPartitionButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    //RESET_AFTER_RECONFIG and SNAPPING_MODE check boxes
    resetAfterReconfigCheckBox = new QCheckBox("RESET_AFTER_RECONFIG", this);
    snappingModeCheckBox = new QCheckBox("SNAPPING_MODE", this);
    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(resetAfterReconfigCheckBox);
    buttonGroup->addButton(snappingModeCheckBox);
    buttonGroup->setExclusive(false);
    snappingModeCheckBox->setChecked(true);
    resetAfterReconfigCheckBox->setChecked(true);

    resetAfterReconfigTooltip = tr("Tras una reconfiguración parcial, permite establecer la lógica del nuevo módulo cargado en su estado inicial.") + "\n";
    resetAfterReconfigTooltip += tr("Si no se activa esta opción será necesario aplicar un reset local a toda la lógica del módulo que requiera un valor inicial determinado.");
    snappingModeTooltip = tr("Permite a Vivado realizar pequeñas modificaciones al área definida para ajustarla a los requisitos de la partición.") + "\n";
    snappingModeTooltip += tr("Altamente recomendado si se ha activado la propiedad RESET_AFTER_RECONFIG.");
    resetAfterReconfigCheckBox->setToolTip(resetAfterReconfigTooltip);
    snappingModeCheckBox->setToolTip(snappingModeTooltip);

    //Connect button signal to the slot
    connect(addPartitionButton, SIGNAL(clicked()), this, SLOT(addPartition()));

    //Hw resources
    hwResourcesLabel = new QLabel(tr("Recursos hardware:"));
    hwResourcesLineEdit = new QLineEdit();
    openFloorplanWidgetButton = new QPushButton(tr("Abrir el selector de área"), addPartitionGroup);
    openFloorplanWidgetButton->setFixedHeight(30);
    connect(openFloorplanWidgetButton, SIGNAL(clicked()), this, SLOT(openFloorplanWidget()));

    spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred);

    //Set the layout
    addPartitionGroupLayout = new QGridLayout();
    //addPartitionGroupLayout->addWidget(idLabel, 0, 0);
    //addPartitionGroupLayout->addWidget(idLineEdit, 0, 1, 1, 5);

    /*addPartitionGroupLayout->addWidget(partiNameLabel, 1, 0);
    addPartitionGroupLayout->addWidget(partiNameLineEdit, 1, 1, 1, 5);
    addPartitionGroupLayout->addWidget(partiInstanceLabel, 2, 0);
    addPartitionGroupLayout->addWidget(partiInstanceLineEdit, 2, 1, 1, 5);
    addPartitionGroupLayout->addWidget(sourceDirLabel, 3, 0);
    addPartitionGroupLayout->addWidget(sourceDirLineEdit, 3, 1, 1, 5);
    addPartitionGroupLayout->addWidget(hwResourcesLabel, 4, 0);
    addPartitionGroupLayout->addWidget(hwResourcesLineEdit, 4, 1, 1, 3);
    addPartitionGroupLayout->addWidget(openFloorplanWidgetButton, 4, 4, 1, 2);
    addPartitionGroupLayout->addWidget(partitionPropertiesLabel, 5, 0);
    addPartitionGroupLayout->addWidget(resetAfterReconfigCheckBox, 5, 1);
    addPartitionGroupLayout->addWidget(snappingModeCheckBox, 5, 2);
    addPartitionGroupLayout->addItem(spacer, 5, 3);
    addPartitionGroupLayout->addWidget(addPartitionButton, 6, 5);
    addPartitionGroupLayout->setAlignment(Qt::AlignRight);
    addPartitionGroup->setLayout(addPartitionGroupLayout);*/

    addPartitionGroupLayout->addWidget(partiNameLabel, 0, 0);
    addPartitionGroupLayout->addWidget(partiNameLineEdit, 0, 1, 1, 5);
    addPartitionGroupLayout->addWidget(partiInstanceLabel, 1, 0);
    addPartitionGroupLayout->addWidget(partiInstanceLineEdit, 1, 1, 1, 5);
    addPartitionGroupLayout->addWidget(sourceDirLabel, 2, 0);
    addPartitionGroupLayout->addWidget(sourceDirLineEdit, 2, 1, 1, 5);
    addPartitionGroupLayout->addWidget(hwResourcesLabel, 3, 0);
    addPartitionGroupLayout->addWidget(hwResourcesLineEdit, 3, 1, 1, 3);
    addPartitionGroupLayout->addWidget(openFloorplanWidgetButton, 3, 4, 1, 2);
    addPartitionGroupLayout->addWidget(partitionPropertiesLabel, 4, 0);
    addPartitionGroupLayout->addWidget(resetAfterReconfigCheckBox, 4, 1);
    addPartitionGroupLayout->addWidget(snappingModeCheckBox, 4, 2);
    addPartitionGroupLayout->addItem(spacer, 4, 3);
    addPartitionGroupLayout->addWidget(addPartitionButton, 5, 5);
    addPartitionGroupLayout->setAlignment(Qt::AlignRight);
    addPartitionGroup->setLayout(addPartitionGroupLayout);
}

void PartitionsWidget::createPartitionsGroup() {
    QStringList tableHeader;

    partitionsGroup = new QGroupBox(tr("Particiones existentes"));

    //Create the table of current partitions
    //tableHeader.append(tr("ID"));
    tableHeader.append(tr("Nombre"));
    tableHeader.append(tr("Instancia"));
    tableHeader.append(tr("Source"));
    tableHeader.append("RESET_AFTER_RECONFIG");
    tableHeader.append("SNAPPING_MODE");
    tableHeader.append(tr("Recursos HW"));
    partitionsTable = new QTableWidget(partitionsGroup);
    partitionsTable->setColumnCount(6);
    partitionsTable->setColumnWidth(0, 100);
    partitionsTable->setColumnWidth(1, 150);
    partitionsTable->setColumnWidth(2, 200);
    partitionsTable->setColumnWidth(3, 190);
    partitionsTable->setColumnWidth(4, 175);
    partitionsTable->setColumnWidth(5, 200);
    partitionsTable->setHorizontalHeaderLabels(tableHeader);
    partitionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    partitionsTable->horizontalHeader()->setStretchLastSection(true);
    partitionsTable->verticalHeader()->hide();
    partitionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    partitionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    partitionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    partitionsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    enablePartitionsContextMenu();

    //Create the layout for the groupbox
    partitionsGroupLayout = new QGridLayout();
    partitionsGroupLayout->addWidget(partitionsTable, 0, 0);
    partitionsGroup->setLayout(partitionsGroupLayout);
}

void PartitionsWidget::addPartitionToDesign() {
    ReconfigPartition partition;
    QString id, name, instance, src;
    QStringList hwResourcesSplited;
    bool resetAfterReconfig, snappingMode;

    //Get the data of the partition
    //id = idLineEdit->text();
    name = partiNameLineEdit->text();
    instance = partiInstanceLineEdit->text();
    src = sourceDirLineEdit->text();
    hwResourcesSplited = hwResourcesLineEdit->text().split(" ");
    resetAfterReconfig = resetAfterReconfigCheckBox->isChecked();
    snappingMode = snappingModeCheckBox->isChecked();

    //partition = ReconfigPartition(id.toStdString(), name.toStdString(), instance.toStdString(), src.toStdString(), resetAfterReconfig, snappingMode);
    partition = ReconfigPartition(name.toStdString(), name.toStdString(), instance.toStdString(), src.toStdString(), resetAfterReconfig, snappingMode);
    for (QStringList::size_type i = 0; i < hwResourcesSplited.size(); i++) {
        partition.addHwResource(hwResourcesSplited[i].toStdString());
    }

    mainDesign->addPartition(partition);
}

void PartitionsWidget::addPartitionToTable() {
    int newRowIndex;
    QString resetAfterReconfigStr, snappingModeStr;
    //QTableWidgetItem *idWItem, *nameWItem, *instanceWItem, *srcWItem, *resetAfterReconfigWItem, *snappingModeWItem, *hwResourcesWItem;
    QTableWidgetItem *nameWItem, *instanceWItem, *srcWItem, *resetAfterReconfigWItem, *snappingModeWItem, *hwResourcesWItem;

    //Get the data of the partition
    resetAfterReconfigStr = resetAfterReconfigCheckBox->isChecked() ? "true" : "false";
    snappingModeStr = snappingModeCheckBox->isChecked() ? "true" : "false";

    //idWItem = new QTableWidgetItem(idLineEdit->text());
    nameWItem = new QTableWidgetItem(partiNameLineEdit->text());
    instanceWItem = new QTableWidgetItem(partiInstanceLineEdit->text());
    srcWItem = new QTableWidgetItem(sourceDirLineEdit->text());
    resetAfterReconfigWItem = new QTableWidgetItem(resetAfterReconfigStr);
    snappingModeWItem = new QTableWidgetItem(snappingModeStr);
    hwResourcesWItem = new QTableWidgetItem(hwResourcesLineEdit->text());

    //Add a new partition to the table
    newRowIndex = partitionsTable->rowCount();
    partitionsTable->insertRow(newRowIndex);
    //partitionsTable->setItem(newRowIndex, 0, idWItem);
    /*partitionsTable->setItem(newRowIndex, 1, nameWItem);
    partitionsTable->setItem(newRowIndex, 2, instanceWItem);
    partitionsTable->setItem(newRowIndex, 3, srcWItem);
    partitionsTable->setItem(newRowIndex, 4, resetAfterReconfigWItem);
    partitionsTable->setItem(newRowIndex, 5, snappingModeWItem);
    partitionsTable->setItem(newRowIndex, 6, hwResourcesWItem);
    */
    partitionsTable->setItem(newRowIndex, 0, nameWItem);
    partitionsTable->setItem(newRowIndex, 1, instanceWItem);
    partitionsTable->setItem(newRowIndex, 2, srcWItem);
    partitionsTable->setItem(newRowIndex, 3, resetAfterReconfigWItem);
    partitionsTable->setItem(newRowIndex, 4, snappingModeWItem);
    partitionsTable->setItem(newRowIndex, 5, hwResourcesWItem);
}


void PartitionsWidget::addPartition() {
    bool lineEditsEmpty = false;

    //Check that the user has introduced all the required data
    //lineEditsEmpty = checkLineEditEmpty(idLineEdit) || lineEditsEmpty;
    lineEditsEmpty = checkLineEditEmpty(partiNameLineEdit) || lineEditsEmpty;
    lineEditsEmpty = checkLineEditEmpty(partiInstanceLineEdit) || lineEditsEmpty;
    lineEditsEmpty = checkLineEditEmpty(hwResourcesLineEdit) || lineEditsEmpty;

    if (!lineEditsEmpty) {
        //Check the partition ID
        if (checkPartitionIDOK()) {
            //addPartitionToDesign();
            addPartitionToTable();

            //Fix the user defined pblock (floorplanning)
            fpWidget.disableAddedHWResources();

            //Clear the lineEdits
            //idLineEdit->clear();
            partiNameLineEdit->clear();
            partiInstanceLineEdit->clear();
            sourceDirLineEdit->clear();
            hwResourcesLineEdit->clear();
            hwResourcesLineEdit->setReadOnly(false);

            //Set the check boxes to enable
            resetAfterReconfigCheckBox->setChecked(true);
            snappingModeCheckBox->setChecked(true);
        }
    }
}

void PartitionsWidget::removePartition() {
    int selectedRow;
    QString partitionID, warningMsg;
    QStringList modulesToRemove, configsToRemove;
    QMessageBox::StandardButton questionRes;

    selectedRow = partitionsTable->currentRow();
    partitionID = partitionsTable->item(selectedRow, 0)->text();
    modulesToRemove = mainWindowParent->getModulesByPartition(partitionID);
    configsToRemove = mainWindowParent->getConfigurationsByPartition(partitionID);

    if (modulesToRemove.size() > 0 || configsToRemove.size() > 0) {
        if (modulesToRemove.size() > 0) {
            warningMsg = tr("Al eliminar la partición") + " \"" + partitionID + "\" " + tr("se eliminarán los siguientes módulos:") + "\n";
            for (QStringList::size_type i = 0; i < modulesToRemove.size(); i++) {
                warningMsg += "\"" + modulesToRemove[i] + "\"\n";
            }
        }
        if (configsToRemove.size() > 0) {
            warningMsg += "\n" + tr("También se eliminarán las siguientes configuraciones:") + "\n";
            for (QStringList::size_type i = 0; i < configsToRemove.size(); i++) {
                warningMsg += "\"" + configsToRemove[i] + "\"\n";
            }
        }

        warningMsg += "\n" + tr("¿Está seguro de que desea eliminar la partición") + " \"" + partitionID + "\"?";

        questionRes = QMessageBox::question(this, tr("¿Eliminar partición?"), warningMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (questionRes == QMessageBox::Yes) {
            mainWindowParent->removeModulesByPartition(partitionID);
            mainWindowParent->removeConfigurationsByPartition(partitionID);
            partitionsTable->removeRow(selectedRow);

            //Enable its hardware resources
            fpWidget.removePartitionArea(selectedRow);
        }
    }
    else {
        partitionsTable->removeRow(selectedRow);

        //Enable its hardware resources
        fpWidget.removePartitionArea(selectedRow);
    }
}

void PartitionsWidget::showPartitionsContextMenu(const QPoint &pos) {
    QPoint globalPosition;
    QMenu menu;
    QItemSelectionModel *selection = partitionsTable->selectionModel();

    if (selection->hasSelection()) {
        globalPosition = partitionsTable->mapToGlobal(pos);

        menu.addAction(tr("Eliminar"), this, SLOT(removePartition()));
        menu.exec(globalPosition);
    }
}

void PartitionsWidget::openFloorplanWidget() {
    bool readOK;
    int returnResult;
    QString hwRes;

    fpWidget.setWindowModality(Qt::WindowModal);

    if (!fpWidget.getBoardIsLoaded()) {
        readOK = fpWidget.loadBoardData(mainWindowParent->getBoard());
    }
    else {
        readOK = true;
    }

    if (readOK) {
        returnResult = fpWidget.exec();

        hwResourcesLineEdit->setReadOnly(true);

        if (returnResult == QDialog::Accepted) {
            hwRes = fpWidget.getHWResources();
            hwResourcesLineEdit->setText(hwRes);
        }
        else {
            fpWidget.clearHWResData();
            hwResourcesLineEdit->setText("");
        }
    }
}
