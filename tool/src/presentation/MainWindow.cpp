#include "MainWindow.h"
#include "CreateBootImageSD.h"
#include "ExportToVivadoSDK.h"
#include "ReconfigurationManagement.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

#include <iostream>

#define INITIAL_DIR "/tmp"
#define GENERAL_INFO_INDEX 0
#define STATIC_PART_INDEX 1
#define PARTITIONS_INDEX 2
#define MODULES_INDEX 3
#define CONFIGURATIONS_INDEX 4
#define BITSTREAMS_INDEX 5


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    partialDesign = false;
    cleanedPrj = false;
    currentFlowIndex = -1;
    currentFlowTreeWidgetIndex = -1;

    createTitle();
    createSelectProjectDirectory();
    createFlowTreeWidget();
    createStackedWidget();
    createButtons();
    createLines();
    createProgressBar();
    createActions();
    createMenus();

    spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred);

    //Create the layout
    layout = new QGridLayout();
    layout->addWidget(titleLabel, 0, 0, 1, 5);
    layout->addWidget(projectDirLabel, 1, 0);
    layout->addWidget(projectDirLineEdit, 1, 1, 1, 2);
    layout->addWidget(projectDirButton, 1, 3);
    layout->addWidget(projectDirAcceptButton, 1, 4);
    layout->addWidget(hLineTop, 2, 0, 1, 5);
    layout->addWidget(flowTreeWidget, 3, 0, 2, 2);
    layout->addWidget(stackedWidget, 3, 2, 1, 3);
    layout->addWidget(continueButton, 4, 4);
    layout->addWidget(hLineBottom, 5, 0, 1, 5);
    layout->addWidget(makeProjectLabel, 6, 0);
    layout->addWidget(makeProjectProgressBar, 6, 1, 1, 2);
    layout->addWidget(cleanButton, 6, 3, Qt::AlignRight);
    layout->addWidget(generateButton, 6, 4, Qt::AlignRight);
    layout->addItem(spacer, 7, 2);
    layout->setVerticalSpacing(10);

    
    projectDirLineEdit->setText("/tmp");

    //Create the main widget
    mainWidget = new QWidget();
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);

    //Set window
    resize(1200, 650);
    showMaximized();
    setWindowTitle(tr("dynDeT: dynamic partial reconfiguration DEsign Tool"));
}

QString MainWindow::getBoard() {
    return generalInfoWidget->getBoard();
}

QStringList MainWindow::getModulesByPartition(QString partitionID) {
    return modulesWidget->getModulesByPartition(partitionID);
}

QStringList MainWindow::getConfigurationsByPartition(QString partitionID) {
    return configurationsWidget->getConfigurationsByPartition(partitionID);
}

QStringList MainWindow::getConfigurationsByModule(QString moduleID) {
    return configurationsWidget->getConfigurationsByModule(moduleID);
}

void MainWindow::removeModulesByPartition(QString partitionID) {
    bool modulesListEmpty;

    modulesListEmpty = modulesWidget->removeModulesByPartition(partitionID);

    if (modulesListEmpty && currentFlowIndex > PARTITIONS_INDEX) {
        // for (int i = currentFlowIndex; i > PARTITIONS_INDEX; i--) {
        //     flowTreeWidget->topLevelItem(i)->setDisabled(true);
        // }
        currentFlowIndex = PARTITIONS_INDEX;
    }

    evalContinueButton();
}

void MainWindow::removeConfigurationsByPartition(QString partitionID) {
    bool configurationsListEmpty;

    configurationsListEmpty = configurationsWidget->removeConfigurationsByPartition(partitionID);

    if (configurationsListEmpty && currentFlowIndex > CONFIGURATIONS_INDEX) {
        // for (int i = currentFlowIndex; i > CONFIGURATIONS_INDEX; i--) {
        //     flowTreeWidget->topLevelItem(i)->setDisabled(true);
        // }
        currentFlowIndex = CONFIGURATIONS_INDEX;
    }

    evalContinueButton();
}

void MainWindow::removeConfigurationsByModule(QString moduleID) {
    bool configurationsListEmpty;

    configurationsListEmpty = configurationsWidget->removeConfigurationsByModule(moduleID);

    if (configurationsListEmpty && currentFlowIndex > CONFIGURATIONS_INDEX) {
        // for (int i = currentFlowIndex; i > CONFIGURATIONS_INDEX; i--) {
        //     flowTreeWidget->topLevelItem(i)->setDisabled(true);
        // }
        currentFlowIndex = CONFIGURATIONS_INDEX;
    }

    evalContinueButton();
}


void MainWindow::createTitle() {
    QFont font;

    titleLabel = new QLabel(tr("dynDeT: dynamic partial reconfiguration DEsign Tool"));
    font = titleLabel->font();
    font.setPointSize(11);
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
}

void MainWindow::createSelectProjectDirectory() {
    projectDirLabel = new QLabel(tr("Directorio del proyecto:"));
    projectDirLineEdit = new QLineEdit();
}

void MainWindow::createFlowTreeWidget() {
    QTreeWidgetItem *header, *generalInfo, *staticPart, *partitions, *modules, *configs, *bitstreams;
    QFont headerFont;

    flowTreeWidget = new QTreeWidget(this);
    flowTreeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    flowTreeWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    flowTreeWidget->setFixedWidth(200);
    flowTreeWidget->setStyleSheet("background-color: #EDEDED; border: 0px solid #EDEDED;");

    header = flowTreeWidget->headerItem();

    if (header == NULL) {
        flowTreeWidget->setHeaderLabel(tr("Flujo de diseño"));
        header = flowTreeWidget->headerItem();
    }
    else {
        header->setText(0, tr("Flujo de diseño"));
    }
    headerFont = header->font(0);
    headerFont.setBold(true);
    header->setFont(0, headerFont);
    //flowTreeWidget->header()->setStyleSheet("QHeaderView::section { background-color: #EDEDED; border: 0px solid #EDEDED; border-bottom: 2px groove #D1D1D1; border-right: 2px groove #D1D1D1; padding: 2px }");

    //Create the items for the flow tree view
    generalInfo = new QTreeWidgetItem();
    generalInfo->setText(0, tr("Información general"));
    staticPart = new QTreeWidgetItem();
    staticPart->setText(0, tr("Parte estática"));
    partitions = new QTreeWidgetItem();
    partitions->setText(0, tr("Particiones reconfigurables"));
    modules = new QTreeWidgetItem();
    modules->setText(0, tr("Módulos reconfigurables"));
    configs = new QTreeWidgetItem();
    configs->setText(0, tr("Configuraciones"));
    bitstreams = new QTreeWidgetItem();
    bitstreams->setText(0, tr("Bitstreams"));

    //Disable the items
    generalInfo->setDisabled(true);
    staticPart->setDisabled(true);
    partitions->setDisabled(true);
    modules->setDisabled(true);
    configs->setDisabled(true);
    bitstreams->setDisabled(true);

    //Add the items to the tree widget
    flowTreeWidget->addTopLevelItem(generalInfo);
    flowTreeWidget->addTopLevelItem(staticPart);
    flowTreeWidget->addTopLevelItem(partitions);
    flowTreeWidget->addTopLevelItem(modules);
    flowTreeWidget->addTopLevelItem(configs);
    flowTreeWidget->addTopLevelItem(bitstreams);

    connect(flowTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(flowTreeWidgetItemClicked(QTreeWidgetItem*,int)));

    //Disable the flow tree widget
    flowTreeWidget->setEnabled(false);
}

void MainWindow::createStackedWidget() {
    generalInfoWidget = new GeneralInfoWidget(this, &mainDesign);
    staticPartWidget = new StaticPartWidget(this, &mainDesign);
    configurationsWidget = new ConfigurationsWidget(this, &mainDesign);
    modulesWidget = new ModulesWidget(this, &mainDesign);
    partitionsWidget = new PartitionsWidget(this, &mainDesign);
    bitstreamsWidget = new BitstreamsWidget(this, &mainDesign);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(generalInfoWidget);
    stackedWidget->addWidget(staticPartWidget);
    stackedWidget->addWidget(partitionsWidget);
    stackedWidget->addWidget(modulesWidget);
    stackedWidget->addWidget(configurationsWidget);
    stackedWidget->addWidget(bitstreamsWidget);

    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    stackedWidget->setEnabled(false);
}

void MainWindow::createButtons() {
    projectDirButton = new QPushButton();
    projectDirButton->setIcon(QIcon::fromTheme("folder"));
    projectDirButton->setFixedHeight(30);
    projectDirButton->setFixedWidth(30);
    projectDirButton->setToolTip(tr("Seleccionar el directorio del proyecto"));
    projectDirAcceptButton = new QPushButton(tr("Aceptar"));
    projectDirAcceptButton->setFixedHeight(30);
    continueButton = new QPushButton(tr("Continuar"));
    continueButton->setFixedHeight(30);
    cleanButton = new QPushButton(tr("Limpiar diseño"));
    cleanButton->setFixedHeight(30);
    //cleanButton->setFixedWidth(200);
    generateButton = new QPushButton(tr("Generar diseño"));
    generateButton->setFixedHeight(30);

    continueButton->setEnabled(false);
    generateButton->setEnabled(false);
    cleanButton->setEnabled(true);

    connect(projectDirButton, SIGNAL(clicked()), this, SLOT(selectProjectDir()));
    connect(projectDirAcceptButton, SIGNAL(clicked()), this, SLOT(acceptProjectDir()));
    connect(continueButton, SIGNAL(clicked()), this, SLOT(cont()));
    connect(generateButton, SIGNAL(clicked()), this, SLOT(generate()));
    connect(cleanButton, SIGNAL(clicked()), this, SLOT(cleanDesign()));
}

void MainWindow::createLines() {
    hLineTop = new QFrame();
    hLineTop->setFrameShape(QFrame::HLine);
    hLineTop->setFrameShadow(QFrame::Sunken);
    hLineBottom = new QFrame();
    hLineBottom->setFrameShape(QFrame::HLine);
    hLineBottom->setFrameShadow(QFrame::Sunken);
}

void MainWindow::createProgressBar() {
    makeProjectProgressBar = new QProgressBar(this);
    makeProjectProgressBar->setVisible(false);

    makeProjectLabel = new QLabel();
    makeProjectLabel->setVisible(false);
}

void MainWindow::createActions() {
    createBootImageSDAct = new QAction(tr("Crear imagen de arranque para SD"), this);
    createBootImageSDAct->setEnabled(false);
    connect(createBootImageSDAct, SIGNAL(triggered()), this, SLOT(openCreateBootImageSD()));

    exportToVivadoSDKAct = new QAction(tr("Exportar hardware a Vivado SDK"), this);
    exportToVivadoSDKAct->setEnabled(false);
    connect(exportToVivadoSDKAct, SIGNAL(triggered()), this, SLOT(openExportToVivadoSDK()));

    openReconfManagementAct = new QAction(tr("Gestión de la reconfiguración"), this);
    openReconfManagementAct->setEnabled(false);
    connect(openReconfManagementAct, SIGNAL(triggered()), this, SLOT(openReconfManagement()));

    aboutAct = new QAction(tr("Acerca de"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(openAbout()));
}

void MainWindow::createMenus() {
    menubar = new QMenuBar();

    setMenuBar(menubar);

    toolsMenu = menuBar()->addMenu(tr("Herramientas"));
    toolsMenu->addAction(createBootImageSDAct);
    toolsMenu->addAction(exportToVivadoSDKAct);
    toolsMenu->addAction(openReconfManagementAct);

    helpMenu = menuBar()->addMenu(tr("Ayuda"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::enableMenus() {
    toolsMenu->setEnabled(true);
    helpMenu->setEnabled(true);
}

void MainWindow::disableMenus() {
    toolsMenu->setEnabled(false);
    helpMenu->setEnabled(false);
}

void MainWindow::selectProjectDir() {
    QString dir;

    dir = QFileDialog::getExistingDirectory(this, tr("Seleccionar el directorio del proyecto"), INITIAL_DIR, QFileDialog::ShowDirsOnly);

    if (dir.size() > 0) {
        dir += "/";
        projectDirLineEdit->setText(dir);
    }
}

void MainWindow::acceptProjectDir() {
    QStringList partitionsIDs;
    QList<QStringList> modules;
    QString projectDir, designXMLPath, msg;
    QMessageBox::StandardButton questionRes;
    bool loadFromXMLOK = false, stop = false;

    projectDir = projectDirLineEdit->text();
    designXMLPath = projectDir + "design.xml";

    if (projectDir.isEmpty()) {
        projectDirLineEdit->setStyleSheet("background: #ffcccc");
    }
    else {
        projectDirLineEdit->setStyleSheet("");

        //Check if there exists a previous generated design
        if (QFileInfo::exists(designXMLPath)) {
            msg = tr("Se ha encontrado un diseño creado con esta herramienta en el directorio introducido, ¿desea cargarlo?") +"\n\n";
            msg += tr("AVISO: al cargar el diseño no podrá modificar la información de la parte estática ni crear nuevas particiones reconfigurables.") + " ";
            msg += tr("Antes de cargar el diseño asegúrese de que éste fue generado correctamente al menos una vez.");

            questionRes = QMessageBox::question(this, tr("¿Cargar diseño anterior?"), msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

            if (questionRes == QMessageBox::Yes) {
                loadFromXMLOK = mainDesign.loadFromXML(designXMLPath.toStdString());

                if (!loadFromXMLOK) {
                    QMessageBox::critical(this, tr("Error"), tr("Error al cargar el diseño."));
                    stop = true;
                }
            }
        }

        if (!stop) {
            //Prevent the project directory to be modified and enable the next section
            // projectDirLineEdit->setEnabled(false);
            // projectDirButton->setEnabled(false);
            // projectDirAcceptButton->setEnabled(false);

            //Current flow index: general info
            currentFlowIndex = GENERAL_INFO_INDEX;

            //Enable the tree view, the stacked widget, and the continue button
            flowTreeWidget->setEnabled(true);
            flowTreeWidget->topLevelItem(GENERAL_INFO_INDEX)->setDisabled(false);
            setCurrentFlowTreeWidgetItem(GENERAL_INFO_INDEX);
            stackedWidget->setEnabled(true);
            stackedWidget->setCurrentIndex(GENERAL_INFO_INDEX);
            evalContinueButton();

            //Update the child widgets
            generalInfoWidget->setProjectDir(projectDir);
            staticPartWidget->setProjectDir(projectDir);
            modulesWidget->setProjectDir(projectDir);

            //Enable the "create boot image" action
            createBootImageSDAct->setEnabled(true);
            //Enable the "export HW to SDK" action
            exportToVivadoSDKAct->setEnabled(true);
            //Enable the reconfiguration management
            openReconfManagementAct->setEnabled(true);

            if (loadFromXMLOK) {
                //Update the child widgets
                generalInfoWidget->loadFromDesign();
                staticPartWidget->loadFromDesign();
                partitionsWidget->loadFromDesign();
                modulesWidget->loadFromDesign();
                partitionsIDs = partitionsWidget->getPartitionsIDs();
                modulesWidget->addPartitionsToComboBox(partitionsIDs);
                configurationsWidget->loadFromDesign();
                modules = modulesWidget->getModulesData();
                configurationsWidget->addModulesData(partitionsIDs, modules);

                partitionsWidget->disableExistingPartitions();
                modulesWidget->disableExistingModules();
                configurationsWidget->disableExistingConfigurations();

                partialDesign = true;
		cleanedPrj = false;
                partitionsWidget->disablePartitionsContextMenu();
                currentFlowIndex = MODULES_INDEX;

                //Disable static widgets and set the current index of flow tree widget
                generalInfoWidget->disableAllElements();
                staticPartWidget->disableAllEditElements();
                partitionsWidget->disableAllEditElements();

                flowTreeWidget->topLevelItem(STATIC_PART_INDEX)->setDisabled(false);
                flowTreeWidget->topLevelItem(PARTITIONS_INDEX)->setDisabled(false);
                flowTreeWidget->topLevelItem(MODULES_INDEX)->setDisabled(false);
                flowTreeWidget->topLevelItem(CONFIGURATIONS_INDEX)->setDisabled(false);
                flowTreeWidget->topLevelItem(BITSTREAMS_INDEX)->setDisabled(false);
                setCurrentFlowTreeWidgetItem(MODULES_INDEX);
                stackedWidget->setCurrentIndex(MODULES_INDEX);
                evalContinueButton();
            }
        }
    }
}

bool MainWindow::checkFlowIndex(int index) {
    bool checkOK;
    QStringList partitionsIDs, newBitstreams;
    QList<QStringList> modules;

    //Check if the data of the current item its OK
    // if(cleanedPrj)
    //   return true;
      
    switch (index) {
    case GENERAL_INFO_INDEX:
        checkOK = generalInfoWidget->check();
        break;
    case STATIC_PART_INDEX:
        checkOK = staticPartWidget->check();
        break;
    case PARTITIONS_INDEX:
        checkOK = partitionsWidget->check();

        if (checkOK) {
            partitionsIDs = partitionsWidget->getPartitionsIDs();
            modules = modulesWidget->getModulesData();
            modulesWidget->addPartitionsToComboBox(partitionsIDs);
            configurationsWidget->addModulesData(partitionsIDs, modules);
        }
        break;
    case MODULES_INDEX:
        checkOK = modulesWidget->check();

        if (checkOK) {
            partitionsIDs = partitionsWidget->getPartitionsIDs();
            modules = modulesWidget->getModulesData();
            configurationsWidget->addModulesData(partitionsIDs, modules);
        }
        break;
    case CONFIGURATIONS_INDEX:
        checkOK = configurationsWidget->check();

        if (checkOK) {
            if (!partialDesign) {
                newBitstreams.append("reference");

                if (configurationsWidget->getBlankingOption()) {
                    newBitstreams.append("blanking");
                }
            }

            newBitstreams.append(configurationsWidget->getNewBitstreams());
            bitstreamsWidget->addBitstreams(newBitstreams);
            //configurationsWidget->clearNewBitstreamsList();
            //configurationsWidget->addBlankingOptionToDesign();
        }
        break;
    case BITSTREAMS_INDEX:
        checkOK = true;
        break;
    default:
        checkOK = false;
        break;
    }

    return checkOK;
}

void MainWindow::setCurrentFlowTreeWidgetItem(int index) {
    QTreeWidgetItem *current, *previous;
    QFont fontOfCurrent, fontOfPrevious;

    if (currentFlowTreeWidgetIndex >= GENERAL_INFO_INDEX && currentFlowTreeWidgetIndex <= BITSTREAMS_INDEX) {
        previous = flowTreeWidget->topLevelItem(currentFlowTreeWidgetIndex);

        fontOfPrevious = previous->font(0);
        fontOfPrevious.setBold(false);
        previous->setFont(0, fontOfPrevious);
        previous->setSelected(false);
    }

    if (index >= GENERAL_INFO_INDEX && index <= BITSTREAMS_INDEX) {
        current = flowTreeWidget->topLevelItem(index);

        fontOfCurrent = current->font(0);
        fontOfCurrent.setBold(true);
        current->setFont(0, fontOfCurrent);
        current->setSelected(true);

        currentFlowTreeWidgetIndex = index;
    }
}

void MainWindow::evalContinueButton() {
    //Enable/disable the continue button
    // if (currentFlowTreeWidgetIndex == currentFlowIndex) {
    //     continueButton->setEnabled(true);
    // }
    // else {
    //     continueButton->setEnabled(false);
    // }
  continueButton->setEnabled(true);
  if (currentFlowTreeWidgetIndex == BITSTREAMS_INDEX) 
    continueButton->setText("Habilitar");
  else{
    continueButton->setText("Continuar");
    generateButton->setEnabled(false);
  }
}

void MainWindow::enableFlowAfterAnError() {
    generalInfoWidget->enableAllElements();
    generalInfoWidget->disableFPGADataElements();
    staticPartWidget->enableAllEditElements();
    partitionsWidget->enableAllEditElements();
    partitionsWidget->enablePartitionsContextMenu();
    modulesWidget->enableAllEditElements();
    modulesWidget->enableModulesContextMenu();
    configurationsWidget->enableConfigurationElements();
    configurationsWidget->enableConfigurationsContextMenu();
    configurationsWidget->enableBlankingOption();
    bitstreamsWidget->enableAllEditElements();

    //Update flow
    currentFlowIndex = BITSTREAMS_INDEX;
    evalContinueButton();
}

void MainWindow::enablePartialFlowAfterAnError() {
    modulesWidget->enableAllEditElements();
    modulesWidget->enableModulesContextMenu();
    configurationsWidget->enableConfigurationElements();
    configurationsWidget->enableConfigurationsContextMenu();
    bitstreamsWidget->enableAllEditElements();

    //Update flow
    currentFlowIndex = BITSTREAMS_INDEX;
    evalContinueButton();
}

void MainWindow::flowTreeWidgetItemClicked(QTreeWidgetItem *item, int column) {
    int itemIndex;
    bool checkOK = false;

    itemIndex = flowTreeWidget->indexOfTopLevelItem(item);
    
    //Checkf if the item is a top item
    if (cleanedPrj){
      currentFlowIndex = itemIndex;
      if (itemIndex !=0)
	checkOK = checkFlowIndex(itemIndex-1);
      else
	checkOK = true;
    }
    else if (item->isDisabled() == false && column == 0) {
	currentFlowIndex = itemIndex;
    	checkOK = checkFlowIndex(currentFlowTreeWidgetIndex);
	
        // if (currentFlowTreeWidgetIndex >= GENERAL_INFO_INDEX && currentFlowTreeWidgetIndex < currentFlowIndex) {
        //     checkOK = checkFlowIndex(currentFlowTreeWidgetIndex);
        // }
        // else {
        //     checkOK = true;
        // }

	//evalContinueButton();
    }
    
    if (checkOK) {
      setCurrentFlowTreeWidgetItem(currentFlowIndex);
      //Update the view
      stackedWidget->setCurrentIndex(currentFlowIndex);
    }
    evalContinueButton();
}


void MainWindow::cont() {
    bool checkOK;

    checkOK = checkFlowIndex(currentFlowIndex);

    // if (currentFlowIndex == GENERAL_INFO_INDEX && checkOK) {
    //     generalInfoWidget->disableFPGADataElements();
    // }


    if (currentFlowTreeWidgetIndex < BITSTREAMS_INDEX && checkOK) {
        //Go to the next widget
        currentFlowIndex += 1;
        flowTreeWidget->topLevelItem(currentFlowIndex)->setDisabled(false);
        setCurrentFlowTreeWidgetItem(currentFlowIndex);
        stackedWidget->setCurrentIndex(currentFlowIndex);

        evalContinueButton();
    }
    else {
        if (currentFlowTreeWidgetIndex == BITSTREAMS_INDEX) {
            //Set the current flow index to finished (-1)
            currentFlowIndex = -1;

            //Disable widgets
            generalInfoWidget->disableAllElements();
            staticPartWidget->disableAllEditElements();
            partitionsWidget->disableAllEditElements();
            partitionsWidget->disablePartitionsContextMenu();
            modulesWidget->disableAllEditElements();
            modulesWidget->disableModulesContextMenu();
            configurationsWidget->disableAllEditElements();
            configurationsWidget->disableConfigurationsContextMenu();
            bitstreamsWidget->disableAllEditElements();

            //Disable the continue button and enable the generate button
            continueButton->setEnabled(false);
            generateButton->setEnabled(true);
        }
    }
}

void MainWindow::cleanDesign() {
  makeProjectLabel->setText(tr("Limpiando..."));
  makeProjectLabel->setVisible(true);

  makeProcess = new QProcess(this);
  makeProcess->setWorkingDirectory(projectDirLineEdit->text());

  makeProcess->start("make clean");
  partialDesign=false;
  cleanedPrj=true;
  makeProjectLabel->setVisible(false);
  generateButton->setEnabled(true);

  
    generalInfoWidget->enableAllElements();
    staticPartWidget->enableAllEditElements();
    partitionsWidget->enableAllEditElements();
    partitionsWidget->enablePartitionsContextMenu();
    modulesWidget->enableAllEditElements();
    modulesWidget->enableModulesContextMenu();
    configurationsWidget->enableConfigurationElements();
    configurationsWidget->enableConfigurationsContextMenu();
    configurationsWidget->enableBlankingOption();
    bitstreamsWidget->enableAllEditElements();

    
    partitionsWidget->enableExistingPartitions();
    modulesWidget->enableExistingModules();
    configurationsWidget->enableExistingConfigurations();
}

void MainWindow::generate() {
  std::string baseDir, outputFileNameTCL, outputFileNameXML;
  bool addModulesOK, addConfigsOK, designTCLOK, designFilesOK = false, designXMLOK = false;

  baseDir = projectDirLineEdit->text().toStdString();
  outputFileNameTCL = baseDir + "design.tcl";
  outputFileNameXML = baseDir + "design.xml";
    // outputFileNameTCL = projectDirLineEdit->text().toStdString() + "design.tcl";
    // outputFileNameXML = projectDirLineEdit->text().toStdString() + "design.xml";

    generateButton->setEnabled(false);

    //Show progress bar
    makeProjectProgressBar->setMinimum(0);
    makeProjectProgressBar->setMaximum(0);
    makeProjectProgressBar->setFixedHeight(15);
    makeProjectProgressBar->setVisible(true);

    //Add all the data into the design object
    mainDesign = Design();
    generalInfoWidget->addFPGADataToDesign();
    generalInfoWidget->addProjectDataToDesign();
    staticPartWidget->addToDesign();
    partitionsWidget->addAllPartitionsToDesign();
    addModulesOK = modulesWidget->addAllModulesToDesign();
    addConfigsOK = configurationsWidget->addAllConfigurationsToDesign();
    configurationsWidget->addBlankingOptionToDesign();
    bitstreamsWidget->addToDesign();

    if (!addModulesOK || ! addConfigsOK) {
        if (!addModulesOK) {
            QMessageBox::critical(this, tr("Error"), tr("Ha ocurrido un error al añadir los módulos al diseño."));
            makeProjectLabel->setVisible(false);
            makeProjectProgressBar->setVisible(false);
            continueButton->setEnabled(true);
        }

        if (!addConfigsOK) {
            QMessageBox::critical(this, tr("Error"), tr("Ha ocurrido un error al añadir las configuraciones al diseño."));
            makeProjectLabel->setVisible(false);
            makeProjectProgressBar->setVisible(false);
            continueButton->setEnabled(true);
        }
    }
    else {
        //Generate design.tcl and design.xml
        makeProjectLabel->setText(tr("Generando design.tcl..."));
        makeProjectLabel->setVisible(true);

        designTCLOK = mainDesign.generateTCL(outputFileNameTCL);
        designXMLOK = mainDesign.generateXML(outputFileNameXML);

	
        //Copiar TCL y makefiles
        makeProjectLabel->setText(tr("Copiando ficheros..."));
        makeProjectLabel->setVisible(true);

	designFilesOK = mainDesign.copyFile(baseDir);
	
	
        if (designTCLOK && designXMLOK && designFilesOK) {
            //Launch make project process
            makeProjectLabel->setText(tr("Generando..."));

            makeProcess = new QProcess(this);
            makeProcess->setWorkingDirectory(projectDirLineEdit->text());
            connect(makeProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(makeProjectFinished(int,QProcess::ExitStatus)));

            if (partialDesign) {
                makeProcess->start("make buildPartial");
            }
            else {
                makeProcess->start("make build");
            }
        }
        else {
            QMessageBox::critical(this, tr("Error"), tr("¡Error al crear los archivos necesarios para la generación del diseño!"));
            makeProjectLabel->setVisible(false);
            makeProjectProgressBar->setVisible(false);
            continueButton->setEnabled(true);
        }
    }

    if (!addModulesOK || !addConfigsOK || !designTCLOK || !designXMLOK || !designFilesOK) {
        if (partialDesign) {
            enablePartialFlowAfterAnError();
        }
        else {
            enableFlowAfterAnError();
        }
    }
}

void MainWindow::makeProjectFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    makeProjectProgressBar->setMinimum(0);
    makeProjectProgressBar->setMaximum(100);
    makeProjectProgressBar->setValue(100);

    if (exitCode == 0 && exitStatus == 0) {
        QMessageBox::information(this, tr("Informacion"), tr("Proyecto generado correctamente.") + "\n\n" + tr("A continuación puede añadir nuevos módulos y configuraciones al diseño sin necesidad de generar todo el proyecto de nuevo."));

        partialDesign = true;
	cleanedPrj = false;
        currentFlowIndex = MODULES_INDEX;

        //Fix the generated partitions, modules and configs
        partitionsWidget->disableExistingPartitions();
        modulesWidget->disableExistingModules();
        configurationsWidget->disableExistingConfigurations();
        //configurationsWidget->clearNewBitstreamsList();

        modulesWidget->enableAllEditElements();
        modulesWidget->enableModulesContextMenu();
        configurationsWidget->enableConfigurationElements();
        configurationsWidget->enableConfigurationsContextMenu();
        bitstreamsWidget->enableAllEditElements();
        // flowTreeWidget->topLevelItem(CONFIGURATIONS_INDEX)->setDisabled(true);
        // flowTreeWidget->topLevelItem(BITSTREAMS_INDEX)->setDisabled(true);
        setCurrentFlowTreeWidgetItem(MODULES_INDEX);
        stackedWidget->setCurrentIndex(MODULES_INDEX);
        evalContinueButton();
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Ha ocurrido un error al generar el proyecto.") + "\n\n" + tr("Código de error:") + " " + QString::number(exitCode) + "\n\n" + tr("Errores:") + "\n" + makeProcess->readAllStandardError());

        if (partialDesign) {
            enablePartialFlowAfterAnError();
        }
        else {
            enableFlowAfterAnError();
        }

        continueButton->setEnabled(true);
    }

    //Hide progress bar
    makeProjectLabel->setVisible(false);
    makeProjectProgressBar->setVisible(false);
}

void MainWindow::openCreateBootImageSD() {
    CreateBootImageSD cBootImg(this, projectDirLineEdit->text(), generalInfoWidget->getPart());

    disableMenus();
    cBootImg.setWindowModality(Qt::WindowModal);
    cBootImg.exec();
    enableMenus();
}

void MainWindow::openExportToVivadoSDK() {
    ExportToVivadoSDK e2SDK(this, projectDirLineEdit->text());

    disableMenus();
    e2SDK.setWindowModality(Qt::WindowModal);
    e2SDK.exec();
    enableMenus();
}

void MainWindow::openReconfManagement() {
  ReconfigurationManagement rmWidget(this, projectDirLineEdit->text(), generalInfoWidget->getBlockDesign(), mainDesign.getDesignName());

    disableMenus();
    rmWidget.setWindowModality(Qt::WindowModal);
    rmWidget.exec();
    enableMenus();
}

void MainWindow::openAbout() {
    QString aboutText;

    aboutText += tr("dynDeT: dynamic partial reconfiguration DEsign Tool") + "\n";
    aboutText += tr("Version 2.0") + "\n\n";
    // aboutText += tr("Trabajo Fin de Grado") + "\n";
    // aboutText += tr("Autor: Javier Sevilla Ballesteros (Javier.Sevilla3@alu.uclm.es)") + "\n";
    // aboutText += tr("Directores: Julián Caba Jiménez, Julio Daniel Dondo Gazzano") + "\n\n";
    aboutText += tr("ARCO Research Group") + "\n";
    aboutText += tr("Escuela Superior de Informática") + "\n";
    aboutText += tr("Universidad de Castilla-La Mancha");

    disableMenus();
    QMessageBox::about(this, tr("Acerca de"), aboutText);
    enableMenus();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    delete menubar;
    event->accept();
}
