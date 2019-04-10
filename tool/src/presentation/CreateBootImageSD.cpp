#include "CreateBootImageSD.h"
#include "ProgramFPGA.h"
#include <algorithm>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

#define BITSTREAMS_FOLDER "Bitstreams/"
#define MAIN_FULL_BITSTREAM "reference.bit"
#define SDK_FOLDER "sdk/"
#define BOOTSD_FOLDER "bootSD/"

CreateBootImageSD::CreateBootImageSD(QWidget *parent, QString projectDir, QString part) : QDialog(parent), mainDir(projectDir), boardPart(part), creatingBootImageNow(false) {
    QFont font;

    //Label
    titleLabel = new QLabel(tr("Crear imagen de arranque para SD"));
    font = titleLabel->font();
    font.setPointSize(11);
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    //Open SDK & create boot image buttons
    openSDKButton = new QPushButton(tr("Abrir Vivado SDK"), this);
    openSDKButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(openSDKButton, SIGNAL(clicked()), this, SLOT(openSDK()));
    createBootImageButton = new QPushButton(tr("Crear imagen"), this);
    createBootImageButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(createBootImageButton, SIGNAL(clicked()), this, SLOT(createBootImage()));

    createAppGroup();
    createFullBitstreamsGroup();
    createOutputDirGroup();

    spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred);

    //Progress bar
    createBootImageProgressBar = new QProgressBar(this);
    createBootImageProgressBar->setVisible(false);

    if (fullBitstreamsCBox->count() == 0) {
        setSelectableElementsEnabled(false);
    }

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(titleLabel, 0, 0);
    layout->addItem(spacer, 1, 1);
    layout->addWidget(openSDKButton, 1, 2);
    layout->addWidget(appGroup, 2, 0, 1, 3);
    layout->addWidget(fullBitstreamsGroup, 3, 0, 1, 3);
    layout->addWidget(outputDirGroup, 4, 0, 1, 3);
    layout->addWidget(createBootImageProgressBar, 5, 0, 1, 2);
    layout->addWidget(createBootImageButton, 5, 2, 1, 1, Qt::AlignRight);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);

    //Window
    setWindowTitle(tr("Crear imagen de arranque para SD - dynDeT: dynamic partial reconfiguration DEsign Tool"));
    resize(750, 550);
}

void CreateBootImageSD::createAppGroup() {
    QDir sdkDir(mainDir + SDK_FOLDER);
    QFileInfoList sdkDirList;
    QModelIndex fileSystemModelIndex;
    QString name;

    appGroup = new QGroupBox(tr("Aplicación"));

    //Label
    selectAppLabel = new QLabel(tr("Seleccione la aplicación que desea añadir a la imagen de arranque:"));

    //Select app tree view
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
    fileSystemModel->setRootPath(sdkDir.absolutePath());

    selectAppTreeView = new QTreeView(this);
    selectAppTreeView->setModel(fileSystemModel);
    selectAppTreeView->setRootIndex(fileSystemModel->index(fileSystemModel->rootPath()));
    selectAppTreeView->setColumnWidth(0, 450);
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

void CreateBootImageSD::createFullBitstreamsGroup() {
    QDir bitstreamsDir(mainDir + BITSTREAMS_FOLDER);
    QFileInfoList bitstreamsList;
    QString fileName;
    bool cont;

    fullBitstreamsGroup = new QGroupBox(tr("Bitstream completo"));

    //Label
    selectBitstream = new QLabel(tr("Seleccione el bitstream completo que desea añadir a la imagen de arranque:"));

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
    fullBitstreamsLayout->addWidget(selectBitstream, 0, 0);
    fullBitstreamsLayout->addWidget(fullBitstreamsCBox, 1, 0);
    fullBitstreamsGroup->setLayout(fullBitstreamsLayout);
}

void CreateBootImageSD::createOutputDirGroup() {
    outputDirGroup = new QGroupBox(tr("Directorio de salida"));

    //Label & line edit
    outputDirLabel = new QLabel(tr("Indique el directorio donde se guardará la imagen de arranque:"));
    outputDirLineEdit = new QLineEdit();

    //Button
    outputDirButton = new QPushButton(outputDirGroup);
    outputDirButton->setIcon(QIcon::fromTheme("folder"));
    outputDirButton->setFixedHeight(30);
    outputDirButton->setFixedWidth(30);
    outputDirButton->setToolTip(tr("Seleccionar el directorio de salida"));
    connect(outputDirButton, SIGNAL(clicked()), this, SLOT(selectOutputDir()));

    //Create the layout
    outputDirLayout = new QGridLayout();
    outputDirLayout->addWidget(outputDirLabel, 0, 0, 1, 2);
    outputDirLayout->addWidget(outputDirLineEdit, 1, 0);
    outputDirLayout->addWidget(outputDirButton, 1, 1);
    outputDirGroup->setLayout(outputDirLayout);
}

void CreateBootImageSD::selectOutputDir() {
    QString dir;

    dir = QFileDialog::getExistingDirectory(this, tr("Seleccionar el directorio de salida"), outputDirLineEdit->text(), QFileDialog::ShowDirsOnly);

    if (dir.size() > 0) {
        dir += "/";
        outputDirLineEdit->setText(dir);
    }
}

void CreateBootImageSD::openSDK() {
    openSDKProcess = new QProcess(this);
    openSDKProcess->setWorkingDirectory(mainDir);
    connect(openSDKProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(openSDKFinished(int,QProcess::ExitStatus)));
    openSDKProcess->start("make runSDKLaunchTCL");
}

void CreateBootImageSD::openSDKFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitCode != 0 || exitStatus != 0) {
        QMessageBox::critical(this, tr("Error"), tr("Error al abrir Vivado SDK.") + "\n\n" + tr("Código de error:") + " " + QString::number(exitCode) + "\n\n" + tr("Errores:") + "\n" + openSDKProcess->readAllStandardError());
    }
}

void CreateBootImageSD::createBootImage() {
    std::string bootSDMKFileName;
    std::vector<std::string> apps;
    QModelIndexList appTreeIndexes;
    QString selectedFileName;
    bool bootSDMKOK;

    bootSDMKFileName = mainDir.toStdString() + BOOTSD_FOLDER + "bootSD.mk";

    appTreeIndexes = selectAppTreeView->selectionModel()->selectedIndexes();

    if (check()) {
        setSelectableElementsEnabled(false);

        //Show progress bar
        createBootImageProgressBar->setMinimum(0);
        createBootImageProgressBar->setMaximum(0);
        createBootImageProgressBar->setFixedHeight(15);
        createBootImageProgressBar->setVisible(true);

        //Generate bootSD.mk
        ProgramFPGA progFPGA(BITSTREAMS_FOLDER);
        progFPGA.setPart(boardPart.left(boardPart.indexOf("-")).toStdString());
        progFPGA.setFullBitstream(fullBitstreamsCBox->currentText().toStdString());

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

        bootSDMKOK = progFPGA.generateBootSDMK(bootSDMKFileName);

        if (bootSDMKOK) {
            //Launch the bootgen process
            creatingBootImageNow = true;
            createBootImageProcess = new QProcess(this);
            createBootImageProcess->setWorkingDirectory(mainDir);
            connect(createBootImageProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(createBootImageFinished(int,QProcess::ExitStatus)));
            createBootImageProcess->start("make genBootSD BOOTSD_OUTPUT_DIR='" + outputDirLineEdit->text()+ "'");
        }
        else {
            QMessageBox::critical(this, tr("Error"), tr("Error al generar el archivo necesario para generar la imagen de arranque.") + "\n\n" + tr("Archivo:") + " " + QString::fromStdString(bootSDMKFileName));

            createBootImageProgressBar->setVisible(false);
            setSelectableElementsEnabled(true);
        }
    }
}

void CreateBootImageSD::createBootImageFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    createBootImageProgressBar->setMinimum(0);
    createBootImageProgressBar->setMaximum(100);
    createBootImageProgressBar->setValue(100);

    if (exitCode == 0 && exitStatus == 0) {
        QMessageBox::information(this, tr("Informacion"), tr("Imagen de arranque generada correctamente."));
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Ha ocurrido un error al crear la imagen de arranque para la tarjeta SD.") + "\n\n" + tr("Código de error:") + " " + QString::number(exitCode) + "\n\n" + tr("Errores:") + "\n" + createBootImageProcess->readAllStandardError());
    }

    createBootImageProgressBar->setVisible(false);
    creatingBootImageNow = false;
    setSelectableElementsEnabled(true);
}

bool CreateBootImageSD::check() {
    QModelIndexList appTreeIndexes;
    bool appOK = false, isDir = false, outputDirOK = false;

    appTreeIndexes = selectAppTreeView->selectionModel()->selectedIndexes();

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

    if (outputDirLineEdit->text().isEmpty() || outputDirLineEdit->text().contains(" ")) {
        outputDirLineEdit->setStyleSheet("background: #ffcccc");
    }
    else {
        outputDirLineEdit->setStyleSheet("");
        outputDirOK = true;
    }

    return appOK && outputDirOK;
}

void CreateBootImageSD::setSelectableElementsEnabled(bool enabled) {
    openSDKButton->setEnabled(enabled);
    selectAppTreeView->setEnabled(enabled);
    fullBitstreamsCBox->setEnabled(enabled);
    outputDirLineEdit->setEnabled(enabled);
    outputDirButton->setEnabled(enabled);
    createBootImageButton->setEnabled(enabled);
}

void CreateBootImageSD::closeEvent(QCloseEvent *event) {
    if (creatingBootImageNow) {
        event->ignore();
    }
    else {
        event->accept();
    }
}

void CreateBootImageSD::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        if (!creatingBootImageNow) {
            QDialog::keyPressEvent(event);
        }
    }
    else {
        QDialog::keyPressEvent(event);
    }
}
