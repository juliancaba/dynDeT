#include "ProgramUserAppWidget.h"
#include "ProgramFPGA.h"
#include <algorithm>
#include <QHeaderView>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QModelIndex>
#include <QMessageBox>
#include <QMenu>
#include <QTextStream>

#define BITSTREAMS_FOLDER "Bitstreams/"
#define MAIN_FULL_BITSTREAM "reference.bit"
#define SDK_FOLDER "sdk/"


ProgramUserAppWidget::ProgramUserAppWidget(QWidget *parent, QString projectDir, QString blockDesignFile) : QWidget(parent), mainDir(projectDir), programmingNow(false), readPartialBitstreamAddrRangeOK(false), partialBitstreamADDRMin(-1), partialBitstreamADDRMax(-1) {
    //Open SDK and program buttons
    openSDKButton = new QPushButton(tr("Abrir Vivado SDK"), this);
    openSDKButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(openSDKButton, SIGNAL(clicked()), this, SLOT(openSDK()));
    programButton = new QPushButton(tr("Programar"), this);
    programButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(programButton, SIGNAL(clicked()), this, SLOT(program()));

    createHardwarePlatformGroup();
    createPartialBitstreamsGroup();
    createAppGroup();
    createFullBitstreamsGroup();
    readPartialBitstreamAddrRange(blockDesignFile);

    //Create the spacer
    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred);

    //Progress bar
    programProgressBar = new QProgressBar(this);
    programProgressBar->setVisible(false);


    if (selectHWDefinitionCBox->count() == 0 || partialBitstreamsCBox->count() == 0) {
        setSelectableElementsEnabled(false);
    }

    //Layout
    layout = new QGridLayout();
    layout->addWidget(openSDKButton, 0, 1, 1, 2, Qt::AlignRight);
    layout->addItem(verticalSpacer, 0, 1);
    layout->addWidget(hardwarePlatformGroup, 1, 0, 1, 3);
    layout->addWidget(partialBitstreamsGroup, 2, 0, 1, 3);
    layout->addWidget(appGroup, 3, 0, 1, 3);
    layout->addWidget(fullBitstreamsGroup, 4, 0, 1, 3);
    layout->addWidget(programProgressBar, 5, 0, 1, 2);
    layout->addWidget(programButton, 5, 2, 1, 1, Qt::AlignRight);
    setLayout(layout);
}

bool ProgramUserAppWidget::getProgrammingNow() {
    return programmingNow;
}

void ProgramUserAppWidget::createHardwarePlatformGroup() {
    QDir sdkDir(mainDir + SDK_FOLDER);
    QFileInfoList sdkDirList;
    QString dirName;

    hardwarePlatformGroup = new QGroupBox(tr("Plataforma hardware"));

    //Label
    selectHWDefinitionLabel = new QLabel(tr("Seleccione la plataforma hardware:"));

    //CBox
    selectHWDefinitionCBox = new QComboBox(this);
    sdkDirList = sdkDir.entryInfoList(QDir::Dirs, QDir::Name);

    for (QFileInfoList::size_type i = 0; i < sdkDirList.size(); i++) {
        dirName = sdkDirList[i].fileName();

        if (dirName.contains("wrapper_hw_platform") ||
	    dirName.contains("wrapper_sys_hw_platform_0")) {
            selectHWDefinitionCBox->addItem(dirName);
        }
    }

    //Create the layout
    hardwarePlatformLayout = new QGridLayout();
    hardwarePlatformLayout->addWidget(selectHWDefinitionLabel, 0, 0);
    hardwarePlatformLayout->addWidget(selectHWDefinitionCBox, 1, 0);
    hardwarePlatformGroup->setLayout(hardwarePlatformLayout);
}

void ProgramUserAppWidget::createPartialBitstreamsGroup() {
    QDir bitstreamsDir(mainDir + BITSTREAMS_FOLDER);
    QFileInfoList bitstreamsList;
    QString fileName;
    QStringList tableHeader;

    partialBitstreamsGroup = new QGroupBox(tr("Bitstreams parciales (cargar a memoria DDR)"));

    //Labels
    bitstreamName = new QLabel(tr("Bitstream:"));
    bitstreamMemAddress = new QLabel(tr("Dirección de memoria (hex):"));

    //List the directory
    bitstreamsList = bitstreamsDir.entryInfoList(QDir::Files, QDir::Name);

    //Create partial bitstreams cbox
    partialBitstreamsCBox = new QComboBox();
    for (QFileInfoList::size_type i = 0; i < bitstreamsList.size(); i++) {
        fileName = bitstreamsList[i].fileName();

        if (fileName.endsWith(".bit") && fileName.contains("pblock")) {
            partialBitstreamsCBox->addItem(fileName);
        }
    }

    //Bitstream address LineEdit
    bitstreamAddrLineEdit = new QLineEdit();

    //Add partial bitstream button
    addPartialBitstreamButton = new QPushButton(tr("Añadir bitstream"), this);
    addPartialBitstreamButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(addPartialBitstreamButton, SIGNAL(clicked()), this, SLOT(addPartialBitstream()));

    //Bitstreams table
    tableHeader.append(tr("Bitstream"));
    tableHeader.append(tr("Dirección de memoria"));
    bitstreamsTable = new QTableWidget(this);
    bitstreamsTable->setColumnCount(2);
    bitstreamsTable->setColumnWidth(0, 400);
    bitstreamsTable->setHorizontalHeaderLabels(tableHeader);
    bitstreamsTable->horizontalHeader()->setStretchLastSection(true);
    bitstreamsTable->verticalHeader()->hide();
    bitstreamsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bitstreamsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bitstreamsTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    bitstreamsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(bitstreamsTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showBitstreamsTableContextMenu(QPoint)));

    //Create the layout
    partialBitstreamsLayout = new QGridLayout();
    partialBitstreamsLayout->addWidget(bitstreamName, 0, 0);
    partialBitstreamsLayout->addWidget(partialBitstreamsCBox, 0, 1);
    partialBitstreamsLayout->addWidget(bitstreamMemAddress, 0, 2);
    partialBitstreamsLayout->addWidget(bitstreamAddrLineEdit, 0, 3);
    partialBitstreamsLayout->addWidget(addPartialBitstreamButton, 1, 3, 1, 1, Qt::AlignRight);
    partialBitstreamsLayout->addWidget(bitstreamsTable, 2, 0, 1, 4);
    partialBitstreamsGroup->setLayout(partialBitstreamsLayout);
}
void ProgramUserAppWidget::readPartialBitstreamAddrRange(QString blockDesignFile) {
    //Read blockDesign TCL file trying to find the valid address range for partial bitstreams
    bool rangeParsed, offsetParsed;
    QString msgError;

    if (blockDesignFile != "") {
        QFile file(blockDesignFile);

        if (file.open(QIODevice::ReadOnly)) {
            QTextStream blockDesignReader(&file);
            QString line, rangeValue, offsetValue;
            QStringList createbdaddrsegSplitted;

            while (!blockDesignReader.atEnd() && !readPartialBitstreamAddrRangeOK) {
                line = blockDesignReader.readLine();

                if (line.contains("create_bd_addr_seg") && line.contains("[get_bd_addr_spaces axi_read_memory_driver_0/m_axi]")) {
                    createbdaddrsegSplitted = line.split(" ");

                    for (QStringList::size_type i = 0; i < createbdaddrsegSplitted.size(); i++) {
                        if (createbdaddrsegSplitted[i] == "-range") {
                            rangeValue = createbdaddrsegSplitted[i + 1];
                        }

                        if (createbdaddrsegSplitted[i] == "-offset") {
                            offsetValue = createbdaddrsegSplitted[i + 1];
                        }
                    }

                    partialBitstreamADDRMin = offsetValue.toULong(&offsetParsed, 16);
                    partialBitstreamADDRMax = partialBitstreamADDRMin + rangeValue.toULong(&rangeParsed, 16) - 0x1;

                    if (offsetParsed && rangeParsed && partialBitstreamADDRMin <= partialBitstreamADDRMax) {
                        readPartialBitstreamAddrRangeOK = true;
                    }
                }
            }

            file.close();
        }
    }

    if(!readPartialBitstreamAddrRangeOK) {
        msgError += tr("Error al leer el rango de direcciones de la memoria DDR donde se pueden cargar los bitstreams parciales.") + "\n";
        msgError += tr("Compruebe que ha especificado el archivo que contiene el diseño de bloques en el flujo de diseño y que este archivo es correcto.") + "\n\n";
        msgError += tr("ATENCIÓN: no se comprobará si las direcciones de memoria introducidas son válidas.");

        QMessageBox::warning(this, tr("AVISO - Rango de direcciones DDR"), msgError);
    }
}

void ProgramUserAppWidget::createAppGroup() {
    QDir sdkDir(mainDir + SDK_FOLDER);
    QFileInfoList sdkDirList;
    QModelIndex fileSystemModelIndex;
    QString name;

    appGroup = new QGroupBox(tr("Aplicación"));

    //Label
    selectAppLabel = new QLabel(tr("Seleccione la aplicación que desea cargar en la memoria del dispositivo:"));

    //Select app tree view
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
    fileSystemModel->setRootPath(sdkDir.absolutePath());

    selectAppTreeView = new QTreeView(this);
    selectAppTreeView->setModel(fileSystemModel);
    selectAppTreeView->setRootIndex(fileSystemModel->index(fileSystemModel->rootPath()));
    selectAppTreeView->setColumnWidth(0, 300);
    selectAppTreeView->hideColumn(2);
    selectAppTreeView->hideColumn(3);

    sdkDirList = sdkDir.entryInfoList(QDir::Dirs | QDir::Files, QDir::Name);
    for (QFileInfoList::size_type i = 0; i < sdkDirList.size(); i++) {
        name = sdkDirList[i].fileName();

        if (name.contains(".bit") || name.contains(".hdf") || name.contains(".log") || name.contains("wrapper_hw_platform") || name.contains("RemoteSystemsTempFiles") || name.contains("webtalk")) {
            fileSystemModelIndex = fileSystemModel->index(sdkDirList[i].absoluteFilePath());
            selectAppTreeView->setRowHidden(fileSystemModelIndex.row(), fileSystemModelIndex.parent(), true);
        }
    }

    //Create the layout
    appLayout = new QGridLayout();
    appLayout->addWidget(selectAppLabel, 0, 0);
    appLayout->addWidget(selectAppTreeView, 1, 0);
    appGroup->setLayout(appLayout);
}

void ProgramUserAppWidget::createFullBitstreamsGroup() {
    QDir bitstreamsDir(mainDir + BITSTREAMS_FOLDER);
    QFileInfoList bitstreamsList;
    QString fileName;
    bool cont;

    fullBitstreamsGroup = new QGroupBox(tr("Bitstream completo"));

    //Label
    selectBitstreamToProgram = new QLabel(tr("Seleccione el bitstream completo con el que programar el dispositivo:"));

    //List the directory
    bitstreamsList = bitstreamsDir.entryInfoList(QDir::Files, QDir::Name);

    //Create full bitstreams cbox
    fullBitstreamsCBox = new QComboBox();
    for (QFileInfoList::size_type i = 0; i < bitstreamsList.size(); i++) {
        fileName = bitstreamsList[i].fileName();

        if (fileName.endsWith(".bit") && !fileName.contains("pblock")) {
            fullBitstreamsCBox->addItem(fileName);
        }
    }

    cont = true;
    for (int i = 0; i < fullBitstreamsCBox->count() && cont; i++) {
        if (fullBitstreamsCBox->itemText(i) == MAIN_FULL_BITSTREAM) {
            fullBitstreamsCBox->setCurrentIndex(i);
            cont = false;
        }
    }

    //Create the layout
    fullBitstreamsLayout = new QGridLayout();
    fullBitstreamsLayout->addWidget(selectBitstreamToProgram, 0, 0);
    fullBitstreamsLayout->addWidget(fullBitstreamsCBox, 1, 0);
    fullBitstreamsGroup->setLayout(fullBitstreamsLayout);
}

bool ProgramUserAppWidget::check() {
    QModelIndexList appTreeIndexes;
    bool bitstreamsOK = false, appOK = false, isDir = false;

    appTreeIndexes = selectAppTreeView->selectionModel()->selectedIndexes();

    if (bitstreamsTable->rowCount() == 0) {
        bitstreamsTable->setStyleSheet("background: #ffcccc");
    }
    else {
        bitstreamsTable->setStyleSheet("");
        bitstreamsOK = true;
    }

    if (appTreeIndexes.size() == 0) {
        selectAppTreeView->setStyleSheet("background: #ffcccc");
    }
    else {
        for (QModelIndexList::size_type i = 0; i < appTreeIndexes.size() && !isDir; i++) {
            if (fileSystemModel->isDir(appTreeIndexes[i])) {
                isDir = true;
            }
        }

        if (isDir) {
            selectAppTreeView->setStyleSheet("background: #ffcccc");
        }
        else {
            selectAppTreeView->setStyleSheet("");
            appOK = true;
        }
    }

    return bitstreamsOK && appOK;
}

bool ProgramUserAppWidget::checkPartialBitstreamAddr() {
    bool result = false, addrParsed;
    long addr;
    QString addrTxt;

    if (!bitstreamAddrLineEdit->text().isEmpty()) {
        addrTxt = bitstreamAddrLineEdit->text();
        addr = addrTxt.toULong(&addrParsed, 16);

        if (addrParsed) {
            if (readPartialBitstreamAddrRangeOK) {
                if (addr >= partialBitstreamADDRMin && addr <= partialBitstreamADDRMax) {
                    result = true;
                }
            }
            else {
                result = true;
            }
        }
    }

    if (result) {
        bitstreamAddrLineEdit->setStyleSheet("");
    }
    else {
        bitstreamAddrLineEdit->setStyleSheet("background: #ffcccc");
    }

    return result;
}

void ProgramUserAppWidget::setSelectableElementsEnabled(bool enabled) {
    openSDKButton->setEnabled(enabled);
    addPartialBitstreamButton->setEnabled(enabled);
    programButton->setEnabled(enabled);
    selectHWDefinitionCBox->setEnabled(enabled);
    partialBitstreamsCBox->setEnabled(enabled);
    bitstreamAddrLineEdit->setEnabled(enabled);
    bitstreamsTable->setEnabled(enabled);
    selectAppTreeView->setEnabled(enabled);
    fullBitstreamsCBox->setEnabled(enabled);
}

void ProgramUserAppWidget::openSDK() {
    openSDKProcess = new QProcess(this);
    openSDKProcess->setWorkingDirectory(mainDir);
    connect(openSDKProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(openSDKFinished(int,QProcess::ExitStatus)));
    openSDKProcess->start("make runSDKLaunchTCL");
}

void ProgramUserAppWidget::openSDKFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitCode != 0 || exitStatus != 0) {
        QMessageBox::critical(this, tr("Error"), tr("Error al abrir Vivado SDK.") + "\n\n" + tr("Código de error:") + " " + QString::number(exitCode) + "\n\n" + tr("Errores:") + "\n" + openSDKProcess->readAllStandardError());
    }
}

void ProgramUserAppWidget::addPartialBitstream() {
    int newRowIndex;
    QTableWidgetItem *nameWItem, *addrWItem;

    if (checkPartialBitstreamAddr()) {
        //Add bitstreams to table
        nameWItem = new QTableWidgetItem(partialBitstreamsCBox->currentText());
        addrWItem = new QTableWidgetItem(bitstreamAddrLineEdit->text());

        newRowIndex = bitstreamsTable->rowCount();
        bitstreamsTable->insertRow(newRowIndex);
        bitstreamsTable->setItem(newRowIndex, 0, nameWItem);
        bitstreamsTable->setItem(newRowIndex, 1, addrWItem);

        bitstreamAddrLineEdit->clear();
    }
}

void ProgramUserAppWidget::showBitstreamsTableContextMenu(const QPoint &pos) {
    QPoint globalPosition;
    QMenu menu;

    if (bitstreamsTable->selectedItems().size() > 0) {
        globalPosition = bitstreamsTable->mapToGlobal(pos);
        menu.addAction(tr("Eliminar"), this, SLOT(removeBitstreamsTableSelectedItems()));
        menu.exec(globalPosition);
    }
}

void ProgramUserAppWidget::removeBitstreamsTableSelectedItems() {
    int index;
    QList<QTableWidgetItem*> itemsToRemove;

    itemsToRemove = bitstreamsTable->selectedItems();

    for (int i = 0; i < itemsToRemove.size(); i++) {
        index = bitstreamsTable->row(itemsToRemove[i]);
        bitstreamsTable->removeRow(index);
    }
}

void ProgramUserAppWidget::program() {
    std::string programTCLFileName, deployTCLFileName, deployMKFileName;
    std::vector<std::string> apps;
    QModelIndexList appTreeIndexes;
    QString selectedFileName;
    bool programTCLOK, deployTCLOK, deployMKOK;

    programTCLFileName = mainDir.toStdString() + "program.tcl";
#if defined VERSION==2015
    deployTCLFileName = mainDir.toStdString() + "xmd.tcl";
#else
    deployTCLFileName = mainDir.toStdString() + "xsdb.tcl";
#endif
    deployMKFileName = mainDir.toStdString() + "deploy.mk";

    appTreeIndexes = selectAppTreeView->selectionModel()->selectedIndexes();

    if (check()) {
        setSelectableElementsEnabled(false);

        //Show progress bar
        programProgressBar->setMinimum(0);
        programProgressBar->setMaximum(0);
        programProgressBar->setFixedHeight(15);
        programProgressBar->setVisible(true);

        //Create ProgramFPGA object
        ProgramFPGA progFPGA(BITSTREAMS_FOLDER);
        progFPGA.setFullBitstream(fullBitstreamsCBox->currentText().toStdString());
        progFPGA.setWrapperHWPlatform(SDK_FOLDER + selectHWDefinitionCBox->currentText().toStdString() + "/ps7_init.tcl");

        for (int i = 0; i < bitstreamsTable->rowCount(); i++) {
            progFPGA.addPartialBitstream(bitstreamsTable->item(i, 0)->text().toStdString(), bitstreamsTable->item(i, 1)->text().toStdString());
        }

        for (QModelIndexList::size_type i = 0; i < appTreeIndexes.size(); i++) {
            selectedFileName = fileSystemModel->filePath(appTreeIndexes[i]);
            selectedFileName = selectedFileName.remove(mainDir);
            //Check if the element was inserted before
            if (std::find(apps.begin(), apps.end(), selectedFileName.toStdString()) == apps.end()) {
                apps.push_back(selectedFileName.toStdString());
            }
        }
        for (size_t i = 0; i < apps.size(); i++) {
            progFPGA.addApplication(apps[i]);
        }

        //Generate program, xmd, and deploy files
        programTCLOK = progFPGA.generateProgramTCL(programTCLFileName);
        deployTCLOK = progFPGA.generateDeployTCL(deployTCLFileName);
        deployMKOK = progFPGA.generateDeployMK(deployMKFileName);

        if (programTCLOK && deployTCLOK && deployMKOK) {
            startProgramFullBitstrProcess();
        }
        else {
            QMessageBox::critical(this, tr("Error"), tr("Error al generar los archivos necesarios para la programación de la FPGA."));
            programProgressBar->setVisible(false);
            setSelectableElementsEnabled(true);
        }
    }
}

void ProgramUserAppWidget::startProgramFullBitstrProcess() {
    //Launch the program (a full bitstream) process
    programmingNow = true;
    programFullBitstrProcess = new QProcess(this);
    programFullBitstrProcess->setWorkingDirectory(mainDir);
    connect(programFullBitstrProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(programFullBitstrProcessFinished(int,QProcess::ExitStatus)));
    programFullBitstrProcess->start("make -f deploy.mk program");
}

void ProgramUserAppWidget::startBuildFactoryBitstrsProcess() {
    //Launch the build factory bitstreams process
    buildFactoryBitstrsProcess = new QProcess(this);
    buildFactoryBitstrsProcess->setWorkingDirectory(mainDir);
    connect(buildFactoryBitstrsProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(buildFactoryBitstrsProcessFinished(int,QProcess::ExitStatus)));
    buildFactoryBitstrsProcess->start("make -f deploy.mk build-partial-bitstreams");
}

void ProgramUserAppWidget::startDeployProcess() {
    //Deploy the user app
    deployProcess = new QProcess(this);
    deployProcess->setWorkingDirectory(mainDir);
    connect(deployProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(deployProcessFinished(int,QProcess::ExitStatus)));
    deployProcess->start("make -f deploy.mk deploy");
}

void ProgramUserAppWidget::programFullBitstrProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitCode == 0 && exitStatus == 0) {
        startBuildFactoryBitstrsProcess();
    }
    else {
        programProgressBar->setMinimum(0);
        programProgressBar->setMaximum(100);
        programProgressBar->setValue(100);

        QMessageBox::critical(this, tr("Programacion detenida"), tr("Ha ocurrido un error al programar el bitstream completo en la FPGA.") + "\n\n" + tr("Código de error:") + " " + QString::number(exitCode) + "\n\n" + tr("Errores:") + "\n" + programFullBitstrProcess->readAllStandardError());

        programProgressBar->setVisible(false);
        programmingNow = false;
        setSelectableElementsEnabled(true);
    }
}

void ProgramUserAppWidget::buildFactoryBitstrsProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitCode == 0 && exitStatus == 0) {
        startDeployProcess();
    }
    else {
        programProgressBar->setMinimum(0);
        programProgressBar->setMaximum(100);
        programProgressBar->setValue(100);

        QMessageBox::critical(this, tr("Programacion detenida"), tr("Ha ocurrido un error al generar los factory bitstreams.") + "\n\n" + tr("Código de error:") + " " + QString::number(exitCode) + "\n\n" + tr("Errores:") + "\n" + buildFactoryBitstrsProcess->readAllStandardError());

        programProgressBar->setVisible(false);
        programmingNow = false;
        setSelectableElementsEnabled(true);
    }
}

void ProgramUserAppWidget::deployProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    programProgressBar->setMinimum(0);
    programProgressBar->setMaximum(100);
    programProgressBar->setValue(100);

    if (exitCode == 0 && exitStatus == 0) {
        QMessageBox::information(this, tr("Informacion"), tr("Aplicación desplegada correctamente."));
    }
    else {
        QMessageBox::critical(this, tr("Programacion detenida"), tr("Ha ocurrido un error al desplegar la aplicación de usuario.") + "\n\n" + tr("Código de error:") + " " + QString::number(exitCode) + "\n\n" + tr("Errores:") + "\n" + deployProcess->readAllStandardError());
    }

    programProgressBar->setVisible(false);
    programmingNow = false;
    setSelectableElementsEnabled(true);
}
