#include "ConfigurationsWidget.h"
#include <QHeaderView>
#include <QComboBox>
#include <QMenu>
#include <QRegularExpression>

#include <iostream>
using namespace std;


ConfigurationsWidget::ConfigurationsWidget(QWidget *parent, Design *des) : QWidget(parent), mainDesign(des) {
    createAddConfigGroup();
    createConfigsGroup();
    createOtherOptionsGroup();

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(addConfigGroup, 0, 0);
    layout->addWidget(configsGroup, 1, 0);
    layout->addWidget(otherOptionsGroup, 2, 0);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);
}

void ConfigurationsWidget::addModulesData(QStringList &partitionsIDs, QList<QStringList> &modData) {
    int newRowIndex;
    QTableWidgetItem *auxPart;
    QComboBox *auxCBox;

    modulesToImplementTable->setRowCount(0);

    //For each partition add its possible modules
    for (QStringList::size_type i = 0; i < partitionsIDs.size(); i++) {
        newRowIndex = modulesToImplementTable->rowCount();
        modulesToImplementTable->insertRow(newRowIndex);

        auxPart = new QTableWidgetItem(partitionsIDs[i]);
        auxCBox = new QComboBox();
        auxCBox->addItem("");

	bool first = true;
        for (QList<QStringList>::size_type j = 0; j < modData.size(); j++) {
            if (partitionsIDs[i] == modData[j][1]) {
	      if (first){
                auxCBox->addItem(modData[j][0]+ " (reference)");
		first=false;
	      }
	      else
		auxCBox->addItem(modData[j][0]);
            }
        }

        modulesToImplementTable->setItem(newRowIndex, 0, auxPart);
        modulesToImplementTable->setCellWidget(newRowIndex, 1, auxCBox);
    }
}

void ConfigurationsWidget::addBlankingOptionToDesign() {
    mainDesign->setCreateBlanking(generateBlankingConfigurationCheckBox->isChecked());
}

bool ConfigurationsWidget::getBlankingOption() {
    return generateBlankingConfigurationCheckBox->isChecked();
}

void ConfigurationsWidget::enableBlankingOption() {
    otherOptionsGroup->setEnabled(true);
}

void ConfigurationsWidget::disableBlankingOption() {
    otherOptionsGroup->setEnabled(false);
}

void ConfigurationsWidget::enableConfigurationElements() {
    addConfigGroup->setEnabled(true);
    //configsGroup->setEnabled(true);
}

void ConfigurationsWidget::disableAllEditElements() {
    addConfigGroup->setEnabled(false);
    //configsGroup->setEnabled(false);
    disableBlankingOption();
}

void ConfigurationsWidget::loadFromDesign() {
    int newRowIndex;
    //QTableWidgetItem *idWItem, *nameWItem, *modulesWItem;
    QTableWidgetItem *nameWItem, *modulesWItem;
    QString modulesStr;
    std::vector<Configuration> configs;
    std::vector<std::string> modulesIDs;

    configs = mainDesign->getConfigurations();

    for (size_t i = 0; i < configs.size(); i++) {
        //Get the data of the config
        //idWItem = new QTableWidgetItem(QString::fromStdString(configs[i].getConfigID()));
        nameWItem = new QTableWidgetItem(QString::fromStdString(configs[i].getName()));

        modulesIDs = configs[i].getModulesIDs();
        for (size_t j = 0; j < modulesIDs.size(); j++) {
            ReconfigModule module = mainDesign->getModule(modulesIDs[j]);

            modulesStr += QString::fromStdString(module.getModuleID());
            modulesStr += " (";
            modulesStr += QString::fromStdString(module.getPartitionID());
            modulesStr += "); ";
        }
        modulesWItem = new QTableWidgetItem(modulesStr);

        //Add a new config to the table
        newRowIndex = configsTable->rowCount();
        configsTable->insertRow(newRowIndex);
        // configsTable->setItem(newRowIndex, 0, idWItem);
        // configsTable->setItem(newRowIndex, 1, nameWItem);
        // configsTable->setItem(newRowIndex, 2, modulesWItem);
        configsTable->setItem(newRowIndex, 0, nameWItem);
        configsTable->setItem(newRowIndex, 1, modulesWItem);

        modulesStr.clear();
    }

    //Partial reconfiguration: disable "generate blanking configuration" option
    otherOptionsGroup->setEnabled(false);
}

bool ConfigurationsWidget::addAllConfigurationsToDesign() {
    bool allOK = true;

    for (int i = 0; i < configsTable->rowCount(); i++) {
        Configuration config;
        QString id, name, module;
        QStringList modulesToImplement;

        //id = configsTable->item(i, 0)->text();
        name = configsTable->item(i, 0)->text();
        modulesToImplement = configsTable->item(i, 1)->text().split("; ");

        //config.setConfigID(id.toStdString());
	config.setConfigID(name.toStdString());
        config.setName(name.toStdString());
        for (int j = 0; j < modulesToImplement.size(); j++) {
            module = modulesToImplement[j].remove(QRegularExpression(" \\(([^)]+)\\)"));

            if (!module.isEmpty() && module != " ") {
                config.addModule(module.toStdString());
            }
        }

        allOK = allOK && mainDesign->addConfiguration(config);
    }

    return allOK;
}

bool ConfigurationsWidget::check() {
    bool empty = false;

    if (configsTable->rowCount() == 0) {
        configsTable->setStyleSheet("background: #ffcccc");
        empty = true;
    }
    else {
        configsTable->setStyleSheet("");
    }

    return !empty;
}

void ConfigurationsWidget::enableConfigurationsContextMenu() {
    connect(configsTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showConfigContextMenu(QPoint)));
}

void ConfigurationsWidget::disableConfigurationsContextMenu() {
    disconnect(configsTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showConfigContextMenu(QPoint)));
}

QStringList ConfigurationsWidget::getConfigurationsByPartition(QString partitionID) {
    QString configInfo;
    QStringList result;

    for (int i = 0; i < configsTable->rowCount(); i++) {
        configInfo = configsTable->item(i, 1)->text();

        if(configInfo.contains("(" + partitionID + ")")) {
            result.append(configsTable->item(i, 0)->text());
        }
    }

    return result;
}

QStringList ConfigurationsWidget::getConfigurationsByModule(QString moduleID) {
    QString configInfo;
    QStringList result;

    for (int i =  0; i < configsTable->rowCount(); i++) {
        configInfo = configsTable->item(i, 1)->text();

        if (configInfo.contains(moduleID + " ")) {
            result.append(configsTable->item(i, 0)->text());
        }
    }

    return result;
}

bool ConfigurationsWidget::removeConfigurationsByPartition(QString partitionID) {
    QString configID, configInfo;
    int indexNewBitstreams;
    bool empty = false;

    for (int i = configsTable->rowCount() - 1; i >= 0; i--) {
        configInfo = configsTable->item(i, 1)->text();

        if (configInfo.contains("(" + partitionID + ")")) {
            configID = configsTable->item(i, 0)->text();
            configsTable->removeRow(i);

            indexNewBitstreams = newBitstreams.indexOf(configID);
            if (indexNewBitstreams != -1) {
                newBitstreams.removeAt(indexNewBitstreams);
            }
        }
    }

    if (configsTable->rowCount() == 0) {
        empty = true;
    }

    return empty;
}

bool ConfigurationsWidget::removeConfigurationsByModule(QString moduleID) {
    QString configID, configInfo;
    int indexNewBitstreams;
    bool empty = false;

    
    for (int i =  0; i < configsTable->rowCount(); i++) {
        configInfo = configsTable->item(i, 1)->text();

	std::cout << configInfo.toStdString() << std::endl;
        if (configInfo.contains(moduleID + " ")) {
            configID = configsTable->item(i, 0)->text();
            configsTable->removeRow(i);

            indexNewBitstreams = newBitstreams.indexOf(configID);
            if (indexNewBitstreams != -1) {
                newBitstreams.removeAt(indexNewBitstreams);
            }
        }
    }

    if (configsTable->rowCount() == 0) {
        empty = true;
    }

    return empty;
}

void ConfigurationsWidget::createAddConfigGroup() {
    QStringList tableHeader;

    addConfigGroup = new QGroupBox(tr("Añadir una nueva configuración"));

    //Labels, LineEdits, and buttons
    //configIDLabel = new QLabel(tr("ID:"));
    configNameLabel = new QLabel(tr("Nombre:"));
    modulesToImplementLabel = new QLabel(tr("Módulos a implementar:"));
    modulesToImplementLabel->setAlignment(Qt::AlignTop);

    //configIDLineEdit = new QLineEdit();
    configNameLineEdit = new QLineEdit();

    addConfigButton = new QPushButton(tr("Añadir configuración"), addConfigGroup);
    addConfigButton->setFixedHeight(30);
    addConfigButton->setFixedWidth(130);
    connect(addConfigButton, SIGNAL(clicked()), this, SLOT(addConfig()));

    //Create the table that allow the user to select the modules to implement in the new configuration
    tableHeader.append(tr("Partición"));
    tableHeader.append(tr("Módulo"));
    modulesToImplementTable = new QTableWidget(addConfigGroup);
    modulesToImplementTable->setColumnCount(2);
    modulesToImplementTable->setColumnWidth(0, 300);
    modulesToImplementTable->setHorizontalHeaderLabels(tableHeader);
    modulesToImplementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    modulesToImplementTable->horizontalHeader()->setStretchLastSection(true);
    modulesToImplementTable->verticalHeader()->hide();
    modulesToImplementTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    modulesToImplementTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    modulesToImplementTable->setSelectionMode(QAbstractItemView::NoSelection);

    addConfigGroupSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred);

    //Create the layout for the groupbox
    addConfigLayout = new QGridLayout();
    //addConfigLayout->addWidget(configIDLabel, 0, 0);
    //addConfigLayout->addWidget(configIDLineEdit, 0, 1, 1, 2);
    /*    addConfigLayout->addWidget(configNameLabel, 1, 0);
    addConfigLayout->addWidget(configNameLineEdit, 1, 1, 1, 2);
    addConfigLayout->addWidget(modulesToImplementLabel, 2, 0);
    addConfigLayout->addWidget(modulesToImplementTable, 2, 1, 2, 2);
    addConfigLayout->addWidget(addConfigButton, 4, 2);
    addConfigLayout->addItem(addConfigGroupSpacer, 4, 1);
    addConfigGroup->setLayout(addConfigLayout);*/
    
    addConfigLayout->addWidget(configNameLabel, 0, 0);
    addConfigLayout->addWidget(configNameLineEdit, 0, 1, 1, 2);
    addConfigLayout->addWidget(modulesToImplementLabel, 1, 0);
    addConfigLayout->addWidget(modulesToImplementTable, 1, 1, 2, 2);
    addConfigLayout->addWidget(addConfigButton, 3, 2);
    addConfigLayout->addItem(addConfigGroupSpacer, 3, 1);
    addConfigGroup->setLayout(addConfigLayout);
}

void ConfigurationsWidget::createConfigsGroup() {
    QStringList tableHeader;

    configsGroup = new QGroupBox(tr("Configuraciones existentes"));

    //Create the table of current configurations
    //tableHeader.append(tr("ID"));
    tableHeader.append(tr("Nombre"));
    tableHeader.append(tr("Módulos a implementar"));
    configsTable = new QTableWidget(configsGroup);
    configsTable->setColumnCount(2);
    //    configsTable->setColumnWidth(0, 100);
    configsTable->setColumnWidth(1, 100);
    configsTable->setHorizontalHeaderLabels(tableHeader);
    configsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    configsTable->horizontalHeader()->setStretchLastSection(true);
    configsTable->verticalHeader()->hide();
    configsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    configsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    configsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    configsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    enableConfigurationsContextMenu();

    //Create the layout for the groupbox
    configsGroupLayout = new QGridLayout();
    configsGroupLayout->addWidget(configsTable, 0, 0);
    configsGroup->setLayout(configsGroupLayout);
}

void ConfigurationsWidget::createOtherOptionsGroup() {
    otherOptionsGroup = new QGroupBox(tr("Otras opciones"));

    //Create the checkbox
    generateBlankingConfigurationCheckBox = new QCheckBox(tr("Generar la configuración blanking"), otherOptionsGroup);
    generateBlankingConfigurationCheckBox->setChecked(false);

    //Create the layout
    otherOptionsGroupLayout = new QGridLayout();
    otherOptionsGroupLayout->addWidget(generateBlankingConfigurationCheckBox, 0, 0);
    otherOptionsGroup->setLayout(otherOptionsGroupLayout);
}

void ConfigurationsWidget::addConfigToDesign() {
    Configuration config;
    QWidget *widgetCBox;
    QComboBox *cBox;

    //Get the data of the configuration
    //config.setConfigID(configIDLineEdit->text().toStdString());
    config.setConfigID(configNameLineEdit->text().toStdString());
    config.setName(configNameLineEdit->text().toStdString());

    for (int i = 0; i < modulesToImplementTable->rowCount(); i++) {
        widgetCBox = modulesToImplementTable->cellWidget(i, 1);
        cBox = qobject_cast<QComboBox *>(widgetCBox);

        if (!cBox->currentText().isEmpty()) {
            config.addModule(cBox->currentText().toStdString());
        }
    }

    mainDesign->addConfiguration(config);
}

void ConfigurationsWidget::addConfigToTable() {
    int newRowIndex;
    QWidget *widgetCBox;
    QComboBox *cBox;
    QString mods;
    //QTableWidgetItem *idWItem, *nameWItem, *modulesWItem;
    QTableWidgetItem *nameWItem, *modulesWItem;

    //Get the data of the configuration
    //idWItem = new QTableWidgetItem(configIDLineEdit->text());
    nameWItem = new QTableWidgetItem(configNameLineEdit->text());

    mods = "";
    for (int i = 0; i < modulesToImplementTable->rowCount(); i++) {
        widgetCBox = modulesToImplementTable->cellWidget(i, 1);
        cBox = qobject_cast<QComboBox *>(widgetCBox);

        if (!cBox->currentText().isEmpty()) {
            mods += cBox->currentText();
            mods += " (";
            mods += modulesToImplementTable->item(i, 0)->text();
            mods += "); ";
        }	
    }
    modulesWItem = new QTableWidgetItem(mods);

    //Add a new config to the table
    newRowIndex = configsTable->rowCount();
    configsTable->insertRow(newRowIndex);
    //configsTable->setItem(newRowIndex, 0, idWItem);
    configsTable->setItem(newRowIndex, 0, nameWItem);
    configsTable->setItem(newRowIndex, 1, modulesWItem);
}

void ConfigurationsWidget::setFlagsConfigurations(QFlags<Qt::ItemFlag> flags) {
    for (int i = 0; i < configsTable->rowCount(); i++) {
        for (int j = 0; j < configsTable->columnCount(); j++) {
            configsTable->item(i, j)->setFlags(flags);
        }
    }
}

void ConfigurationsWidget::disableExistingConfigurations() {
  setFlagsConfigurations(Qt::ItemIsEnabled);
}

void ConfigurationsWidget::enableExistingConfigurations() {
  setFlagsConfigurations((Qt::ItemIsEnabled | Qt::ItemIsSelectable));
}

void ConfigurationsWidget::addConfig() {
  bool infoEmpty = false, configIDOK = true, configImplOK = true;
    QString newStyle = "background: #ffcccc", configID;
    QWidget *widgetCBox;
    QComboBox *cBox;

    //Check that the user has introduced all the required data
    // if (configIDLineEdit->text().isEmpty()) {
    //     configIDLineEdit->setStyleSheet(newStyle);
    //     infoEmpty = true;
    // }
    // else {
    //     configIDLineEdit->setStyleSheet("");
    // }

    if (configNameLineEdit->text().isEmpty()) {
        configNameLineEdit->setStyleSheet(newStyle);
        infoEmpty = true;
    }
    else {
        configNameLineEdit->setStyleSheet("");
    }


    if (!infoEmpty) {
        //Check the config ID
        //configID = configIDLineEdit->text();
      configID = configNameLineEdit->text();
        for (int i = 0; i < configsTable->rowCount() && configIDOK; i++) {
            if (configsTable->item(i, 0)->text() == configID) {
                configIDOK = false;
            }
        }

	for (int i = 0; i < modulesToImplementTable->rowCount(); i++) {
        widgetCBox = modulesToImplementTable->cellWidget(i, 1);
        cBox = qobject_cast<QComboBox *>(widgetCBox);

        if (cBox->currentText().isEmpty())
	  configImplOK = false;
    }

	if (configImplOK) {
        if (configIDOK) {
	  //configIDLineEdit->setStyleSheet("");
	  configNameLineEdit->setStyleSheet("");
	  modulesToImplementTable->setStyleSheet("");

            //addConfigToDesign();
            addConfigToTable();

            //Add the new config (bitstream) to the list
            newBitstreams.append(configNameLineEdit->text());

            //Clear the lineEdits
            //configIDLineEdit->clear();
            configNameLineEdit->clear();
        }
        else {
	  //configIDLineEdit->setStyleSheet(newStyle);
	  configNameLineEdit->setStyleSheet(newStyle);
        }
	}
        else {  
	  modulesToImplementTable->setStyleSheet(newStyle);
        }
    }
}

void ConfigurationsWidget::removeConfig() {
    int selectedRow, indexNewBitstreams;
    QString configID;

    selectedRow = configsTable->currentRow();
    configID = configsTable->item(selectedRow, 0)->text();

    configsTable->removeRow(selectedRow);

    indexNewBitstreams = newBitstreams.indexOf(configID);
    if (indexNewBitstreams != -1) {
        newBitstreams.removeAt(indexNewBitstreams);
    }
}

void ConfigurationsWidget::showConfigContextMenu(const QPoint &pos) {
    QPoint globalPosition;
    QMenu menu;
    QItemSelectionModel *selection = configsTable->selectionModel();

    if (selection->hasSelection()) {
        globalPosition = configsTable->mapToGlobal(pos);

        menu.addAction(tr("Eliminar"), this, SLOT(removeConfig()));
        menu.exec(globalPosition);
    }
}

QStringList ConfigurationsWidget::getNewBitstreams() {
    return newBitstreams;
}

void ConfigurationsWidget::clearNewBitstreamsList() {
    newBitstreams.clear();
}
