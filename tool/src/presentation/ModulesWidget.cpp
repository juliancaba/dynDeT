#include "ModulesWidget.h"
#include "MainWindow.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>

#include <iostream>


ModulesWidget::ModulesWidget(MainWindow *parent, Design *des) : QWidget(parent), mainDesign(des), mainWindowParent(parent) {
    createAddModuleGroup();
    createModulesGroup();

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(addModuleGroup, 0, 0);
    layout->addWidget(modulesGroup, 1, 0);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);
}

void ModulesWidget::setProjectDir(QString &str) {
    mainDir = str;
}

void ModulesWidget::addPartitionsToComboBox(QStringList &qstrList) {
    modPartitionComboBox->clear();
    modPartitionComboBox->addItems(qstrList);
}

void ModulesWidget::enableAllEditElements() {
    addModuleGroup->setEnabled(true);
    //modulesGroup->setEnabled(true);
}

void ModulesWidget::disableAllEditElements() {
    addModuleGroup->setEnabled(false);
    //modulesGroup->setEnabled(false);
}

QList<QStringList> ModulesWidget::getModulesData() {
    QTableWidgetItem *idItem;
    QList<QStringList> result;

    for (int i = 0; i < modulesTable->rowCount(); i++) {
        QStringList mod;

	idItem = modulesTable->item(i, 0);
        mod.append(idItem->text());
        idItem = modulesTable->item(i, 1);
        mod.append(idItem->text());

        result.append(mod);
    }

    return result;
}

void ModulesWidget::loadFromDesign() {
    int newRowIndex;
    //QTableWidgetItem *idWItem, *nameWItem, *partitionWItem, *sourcesWItem;
    QTableWidgetItem *nameWItem, *partitionWItem, *sourcesWItem;
    QString sourcesStr;
    std::vector<std::vector<std::string> > files;
    std::vector<ReconfigModule> modules;

    modules = mainDesign->getModules();

    for (size_t i = 0; i < modules.size(); i++) {
        //Get the data of the module
        //idWItem = new QTableWidgetItem(QString::fromStdString(modules[i].getModuleID()));
        nameWItem = new QTableWidgetItem(QString::fromStdString(modules[i].getName()));
        partitionWItem = new QTableWidgetItem(QString::fromStdString(modules[i].getPartitionID()));

        files = modules[i].getFiles();
        for (size_t j = 0; j < files.size(); j++) {
            sourcesStr += QString::fromStdString(files[j][0]) + QString::fromStdString(files[j][1]) + " ; ";
        }
        sourcesWItem = new QTableWidgetItem(sourcesStr);

        //Add a new module to the table
        newRowIndex = modulesTable->rowCount();
        modulesTable->insertRow(newRowIndex);
        //modulesTable->setItem(newRowIndex, 0, idWItem);
        /*modulesTable->setItem(newRowIndex, 1, nameWItem);
        modulesTable->setItem(newRowIndex, 2, partitionWItem);
        modulesTable->setItem(newRowIndex, 3, sourcesWItem);*/
	modulesTable->setItem(newRowIndex, 0, nameWItem);
        modulesTable->setItem(newRowIndex, 1, partitionWItem);
        modulesTable->setItem(newRowIndex, 2, sourcesWItem);
	
        sourcesStr.clear();
    }
}

bool ModulesWidget::addAllModulesToDesign() {
    bool allOK = true;

    for (int i = 0; i < modulesTable->rowCount(); i++) {
        ReconfigModule module;
        //QString id, name, partition, auxFilePath, auxFileName;
	QString name, partition, auxFilePath, auxFileName;
        QStringList sources;

        //Get the data of the module
        //id = modulesTable->item(i, 0)->text();
        name = modulesTable->item(i, 0)->text();
        partition = modulesTable->item(i, 1)->text();
        sources = modulesTable->item(i, 2)->text().split(" ; ");

        //module = ReconfigModule(id.toStdString(), name.toStdString(), partition.toStdString());
	module = ReconfigModule(name.toStdString(), name.toStdString(), partition.toStdString());
        for (int j = 0; j < sources.size(); j++) {
            if (!sources[j].isEmpty()) {
                auxFilePath = getPathOfFile(sources[j]);
                auxFileName = getNameOfFile(sources[j]);

                module.addFile(auxFilePath.toStdString(), auxFileName.toStdString());
            }
        }

        allOK = allOK && mainDesign->addModule(module);
    }

    return allOK;
}

bool ModulesWidget::check() {
    bool empty = false;

    if (modulesTable->rowCount() == 0) {
        modulesTable->setStyleSheet("background: #ffcccc");
        empty = true;
    }
    else {
        modulesTable->setStyleSheet("");
    }

    return !empty;
}

void ModulesWidget::enableModulesContextMenu() {
    connect(modulesTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showModulesContextMenu(QPoint)));
}

void ModulesWidget::disableModulesContextMenu() {
    disconnect(modulesTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showModulesContextMenu(QPoint)));
}

void ModulesWidget::setFlagsModules(QFlags<Qt::ItemFlag> flags) {
    for (int i = 0; i < modulesTable->rowCount(); i++) {
        for (int j = 0; j < modulesTable->columnCount(); j++) {
            modulesTable->item(i, j)->setFlags(flags);
        }
    }
}


void ModulesWidget::disableExistingModules() {
  setFlagsModules(Qt::ItemIsEnabled);
}


void ModulesWidget::enableExistingModules() {
  setFlagsModules((Qt::ItemIsEnabled | Qt::ItemIsSelectable));
}


QStringList ModulesWidget::getModulesByPartition(QString partitionID) {
    QString modulePartition;
    QStringList result;

    for (int i = 0; i < modulesTable->rowCount(); i++) {
        modulePartition = modulesTable->item(i, 2)->text();

        if (modulePartition == partitionID) {
            result.append(modulesTable->item(i, 0)->text());
        }
    }

    return result;
}

bool ModulesWidget::removeModulesByPartition(QString partitionID) {
    QString modulePartition;
    bool empty = false;

    for (int i = modulesTable->rowCount() - 1; i >= 0; i--) {
        modulePartition = modulesTable->item(i, 1)->text();
        if (modulePartition == partitionID) {
            modulesTable->removeRow(i);
        }
    }

    if (modulesTable->rowCount() == 0) {
        empty = true;
    }

    return empty;
}

void ModulesWidget::createAddModuleGroup() {
    addModuleGroup = new QGroupBox(tr("Añadir un nuevo módulo"));

    //   modIDLabel = new QLabel(tr("ID:"));
    modNameLabel = new QLabel(tr("Nombre:"));
    modPartitionLabel = new QLabel(tr("Partición:"));
    modFilesLabel = new QLabel(tr("Archivos:"));

    //modIDLineEdit = new QLineEdit();
    modNameLineEdit = new QLineEdit();
    modPartitionComboBox = new QComboBox();

    addFileButton = new QPushButton(tr("Añadir"), addModuleGroup);
    addFileButton->setIcon(QIcon::fromTheme("list-add"));
    addFileButton->setFixedHeight(30);
    addFileButton->setFixedWidth(85);
    addModuleButton = new QPushButton(tr("Añadir módulo"), addModuleGroup);
    addModuleButton->setFixedHeight(30);
    addModuleButton->setFixedWidth(120);

    //Connect button signals to the slots
    connect(addFileButton, SIGNAL(clicked()), this, SLOT(addModuleFiles()));
    connect(addModuleButton, SIGNAL(clicked()), this, SLOT(addModule()));

    //Create the list of sources (new module)
    modFilesList = new QListWidget(addModuleGroup);
    modFilesList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    modFilesList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(modFilesList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showFileListContextMenu(QPoint)));

    addModuleGroupSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred);

    //Set the layout
    addModuleLayout = new QGridLayout();
    //addModuleLayout->addWidget(modIDLabel, 0, 0);
    //addModuleLayout->addWidget(modIDLineEdit, 0, 1, 1, 2);
    /*addModuleLayout->addWidget(modNameLabel, 1, 0);
    addModuleLayout->addWidget(modNameLineEdit, 1, 1, 1, 2);
    addModuleLayout->addWidget(modPartitionLabel, 2, 0);
    addModuleLayout->addWidget(modPartitionComboBox, 2, 1, 1, 2);
    addModuleLayout->addWidget(modFilesLabel, 3, 0);
    addModuleLayout->addWidget(addFileButton, 3, 1);
    addModuleLayout->addWidget(modFilesList, 4, 1, 1, 2);
    addModuleLayout->addWidget(addModuleButton, 5, 2);
    addModuleLayout->addItem(addModuleGroupSpacer, 5, 1);
    addModuleGroup->setLayout(addModuleLayout);*/
    addModuleLayout->addWidget(modNameLabel, 0, 0);
    addModuleLayout->addWidget(modNameLineEdit, 0, 1, 1, 2);
    addModuleLayout->addWidget(modPartitionLabel, 1, 0);
    addModuleLayout->addWidget(modPartitionComboBox, 1, 1, 1, 2);
    addModuleLayout->addWidget(modFilesLabel, 2, 0);
    addModuleLayout->addWidget(addFileButton, 2, 1);
    addModuleLayout->addWidget(modFilesList, 3, 1, 1, 2);
    addModuleLayout->addWidget(addModuleButton, 4, 2);
    addModuleLayout->addItem(addModuleGroupSpacer, 4, 1);
    addModuleGroup->setLayout(addModuleLayout);
}

void ModulesWidget::createModulesGroup() {
    QStringList tableHeader;

    modulesGroup = new QGroupBox(tr("Módulos existentes"));

    //Create the table of current modules
    //tableHeader.append(tr("ID"));
    tableHeader.append(tr("Nombre"));
    tableHeader.append(tr("Partición reconfigurable"));
    tableHeader.append(tr("Sources"));
    modulesTable = new QTableWidget(modulesGroup);
    modulesTable->setColumnCount(3);
    modulesTable->setColumnWidth(0, 100);
    modulesTable->setColumnWidth(1, 150);
    modulesTable->setColumnWidth(2, 180);
    modulesTable->setHorizontalHeaderLabels(tableHeader);
    modulesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    modulesTable->horizontalHeader()->setStretchLastSection(true);
    modulesTable->verticalHeader()->hide();
    modulesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    modulesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    modulesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    modulesTable->setContextMenuPolicy(Qt::CustomContextMenu);
    enableModulesContextMenu();

    //Create the layout for the groupbox
    modulesGroupLayout = new QGridLayout();
    modulesGroupLayout->addWidget(modulesTable, 0, 0);
    modulesGroup->setLayout(modulesGroupLayout);
}

void ModulesWidget::addModuleToDesign() {
    ReconfigModule module;
    //QString id, name, partition, auxFile, auxFilePath, auxFileName;
    QString name, partition, auxFile, auxFilePath, auxFileName;

    //Get the data of the module
    //id = modIDLineEdit->text();
    name = modNameLineEdit->text();
    partition = modPartitionComboBox->currentText();

    //module = ReconfigModule(id.toStdString(), name.toStdString(), partition.toStdString());
    module = ReconfigModule(name.toStdString(), name.toStdString(), partition.toStdString());
    for (int i = 0; i < modFilesList->count(); i++) {
        auxFile = modFilesList->item(i)->text();
        auxFilePath = getPathOfFile(auxFile);
        auxFileName = getNameOfFile(auxFile);

        module.addFile(auxFilePath.toStdString(), auxFileName.toStdString());
    }

    mainDesign->addModule(module);
}

void ModulesWidget::addModuleToTable() {
    int newRowIndex;
    QString srcs;
    //QTableWidgetItem *idWItem, *nameWItem, *partitionWItem, *sourcesWItem;
    QTableWidgetItem *nameWItem, *partitionWItem, *sourcesWItem;

    //Get the data of the module
    //idWItem = new QTableWidgetItem(modIDLineEdit->text());
    nameWItem = new QTableWidgetItem(modNameLineEdit->text());
    partitionWItem = new QTableWidgetItem(modPartitionComboBox->currentText());

    srcs = "";
    for (int i = 0; i < modFilesList->count(); i++) {
        srcs += modFilesList->item(i)->text();
        srcs += " ; ";
    }
    sourcesWItem = new QTableWidgetItem(srcs);

    //Add a new module to the table
    newRowIndex = modulesTable->rowCount();
    modulesTable->insertRow(newRowIndex);
    /*modulesTable->setItem(newRowIndex, 0, idWItem);
    modulesTable->setItem(newRowIndex, 1, nameWItem);
    modulesTable->setItem(newRowIndex, 2, partitionWItem);
    modulesTable->setItem(newRowIndex, 3, sourcesWItem);*/
    
    modulesTable->setItem(newRowIndex, 0, nameWItem);
    modulesTable->setItem(newRowIndex, 1, partitionWItem);
    modulesTable->setItem(newRowIndex, 2, sourcesWItem);
}

QString ModulesWidget::getPathOfFile(QString &file) {
    int index;
    QString res;

    index = file.lastIndexOf("/");
    res = file.left(index + 1);

    return res;
}

QString ModulesWidget::getNameOfFile(QString &file) {
    int index;
    QString res;

    index = file.lastIndexOf("/");
    res = file.right(file.size() - index - 1);

    return res;
}


void ModulesWidget::addModule() {
    bool infoEmpty = false, moduleIDOK = true;
    QString newStyle = "background: #ffcccc", moduleID;

    //Check that the user has introduced all the required data
    // if (modIDLineEdit->text().isEmpty()) {
    //     modIDLineEdit->setStyleSheet(newStyle);
    //     infoEmpty = true;
    // }
    // else {
    //     modIDLineEdit->setStyleSheet("");
    // }

    if (modNameLineEdit->text().isEmpty()) {
        modNameLineEdit->setStyleSheet(newStyle);
        infoEmpty = true;
    }
    else {
        modNameLineEdit->setStyleSheet("");
    }

    if (modFilesList->count() == 0) {
        modFilesList->setStyleSheet(newStyle);
        infoEmpty = true;
    }
    else {
        modFilesList->setStyleSheet("");
    }


    if (!infoEmpty) {
        //Check the module ID
        //moduleID = modIDLineEdit->text();
        moduleID = modNameLineEdit->text();
        for (int i = 0; i < modulesTable->rowCount() && moduleIDOK; i++) {
            if (modulesTable->item(i, 0)->text() == moduleID) {
                moduleIDOK = false;
            }
        }

        if (moduleIDOK) {
	  //modIDLineEdit->setStyleSheet("");
	  modNameLineEdit->setStyleSheet("");

            //addModuleToDesign();
            addModuleToTable();

            //Clear the lineEdits and the list
            //modIDLineEdit->clear();
            modNameLineEdit->clear();
            modPartitionComboBox->setCurrentIndex(0);
            modFilesList->clear();
        }
        else {
	  //modIDLineEdit->setStyleSheet(newStyle);
	  modNameLineEdit->setStyleSheet(newStyle);
        }


    }
}

void ModulesWidget::removeModule() {
    int selectedRow;
    QString moduleID, warningMsg;
    QStringList configsToRemove;
    QMessageBox::StandardButton questionRes;

    selectedRow = modulesTable->currentRow();
    moduleID = modulesTable->item(selectedRow, 0)->text();
    configsToRemove = mainWindowParent->getConfigurationsByModule(moduleID);

    if (configsToRemove.size() > 0) {
        warningMsg = tr("Al eliminar el módulo") + " \"" + moduleID + "\" " + tr("se eliminarán las siguientes configuraciones:") + "\n";
        for (QStringList::size_type i = 0; i < configsToRemove.size(); i++) {
            warningMsg += "\"" + configsToRemove[i] + "\"\n";
        }

        warningMsg += "\n" + tr("¿Está seguro de que desea eliminar el módulo") + " \"" + moduleID + "\"?";

        questionRes = QMessageBox::question(this, tr("¿Eliminar módulo?"), warningMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (questionRes == QMessageBox::Yes) {
	    mainWindowParent->removeConfigurationsByModule(moduleID);
            modulesTable->removeRow(selectedRow);
        }
    }
    else {
        modulesTable->removeRow(selectedRow);
    }
}

void ModulesWidget::showModulesContextMenu(const QPoint &pos) {
    QPoint globalPosition;
    QMenu menu;
    QItemSelectionModel *selection = modulesTable->selectionModel();

    if (selection->hasSelection()) {
        globalPosition = modulesTable->mapToGlobal(pos);

        menu.addAction("Eliminar", this, SLOT(removeModule()));
        menu.exec(globalPosition);
    }
}

void ModulesWidget::addModuleFiles() {
    QStringList files;

    //Open QFileDialog
    files = QFileDialog::getOpenFileNames(this, tr("Seleccionar los archivos del módulo reconfigurable"), mainDir, "All files (*.*)");

    //Add each file tot he list
    for (QStringList::size_type i = 0; i < files.size(); i++) {
        modFilesList->addItem(files[i].remove(mainDir));
    }
}

void ModulesWidget::showFileListContextMenu(const QPoint &pos) {
    QPoint globalPosition;
    QMenu menu;

    if (modFilesList->selectedItems().size() > 0) {
        globalPosition = modFilesList->mapToGlobal(pos);

        menu.addAction(tr("Eliminar"), this, SLOT(removeFileListSelectedItems()));
        menu.exec(globalPosition);
    }
}

void ModulesWidget::removeFileListSelectedItems() {
    QListWidgetItem *item;
    QList<QListWidgetItem*> itemsToRemove;

    itemsToRemove = modFilesList->selectedItems();

    for (int i = 0; i < itemsToRemove.size(); i++) {
        item = modFilesList->takeItem(modFilesList->row(itemsToRemove[i]));
        delete item;
    }
}
