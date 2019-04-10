#include "GeneralInfoWidget.h"
#include <QFileDialog>

GeneralInfoWidget::GeneralInfoWidget(QWidget *parent, Design *des) : QWidget(parent), mainDesign(des) {
    createFPGADataGroup();
    createProjectDataGroup();

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(fpgaDataGroup, 0, 0);
    layout->addWidget(projectDataGroup, 1, 0);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);
}

QString GeneralInfoWidget::getPart() {
    return partComboBox->currentText();
}

QString GeneralInfoWidget::getBoard() {
    return boardComboBox->currentText();
}

QString GeneralInfoWidget::getBlockDesign() {
    return blockDesignLineEdit->text();
}

void GeneralInfoWidget::setProjectDir(QString &str) {
    mainDir = str;
}

void GeneralInfoWidget::enableAllElements() {
    fpgaDataGroup->setEnabled(true);
    projectDataGroup->setEnabled(true);
}

void GeneralInfoWidget::disableFPGADataElements() {
    fpgaDataGroup->setEnabled(false);
}

void GeneralInfoWidget::disableAllElements() {
    fpgaDataGroup->setEnabled(false);
    projectDataGroup->setEnabled(false);
}

void GeneralInfoWidget::addToDesign() {
    addFPGADataToDesign();
    addProjectDataToDesign();
}

void GeneralInfoWidget::addFPGADataToDesign() {
    mainDesign->setPart(partComboBox->currentText().toStdString());
    mainDesign->setBoard(boardComboBox->currentText().toStdString());
}

void GeneralInfoWidget::addProjectDataToDesign() {
  //mainDesign->setProjectName(projectNameLineEdit->text().toStdString());
    mainDesign->setDesignName(designNameLineEdit->text().toStdString());
    mainDesign->setIPDir(ipDirLineEdit->text().toStdString());
    mainDesign->setBlockDesignTCL(blockDesignLineEdit->text().toStdString());
}

void GeneralInfoWidget::loadFromDesign() {
    partComboBox->setCurrentText(QString::fromStdString(mainDesign->getPart()));
    boardComboBox->setCurrentText(QString::fromStdString(mainDesign->getBoard()));
    //projectNameLineEdit->setText(QString::fromStdString(mainDesign->getProjectName()));
    designNameLineEdit->setText(QString::fromStdString(mainDesign->getDesignName()));
    ipDirLineEdit->setText(QString::fromStdString(mainDesign->getIPDir()));
    blockDesignLineEdit->setText(QString::fromStdString(mainDesign->getBlockDesignTCL()));
}

bool GeneralInfoWidget::check() {
    bool empty = false;

    //empty = checkLineEditEmpty(projectNameLineEdit) || empty;
    empty = checkLineEditEmpty(designNameLineEdit) || empty;
    //empty = checkLineEditEmpty(ipDirLineEdit) || empty;
    empty = checkLineEditEmpty(blockDesignLineEdit) || empty;

    return !empty;
}

bool GeneralInfoWidget::checkLineEditEmpty(QLineEdit *le) {
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


void GeneralInfoWidget::createFPGADataGroup() {
    fpgaDataGroup = new QGroupBox(tr("Datos de la FPGA"));

    partLabel = new QLabel(tr("Part:"));
    boardLabel = new QLabel(tr("Board:"));
    partComboBox = new QComboBox();
    partComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(partComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(listCompatibleBoards(QString)));
    boardComboBox = new QComboBox();
    boardComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    //Add compatible parts
    //ZedBoard
    partComboBox->addItem("xc7z020clg484-1");
    //Kintex-7 KC705 (XC7K325T-2FFG900C)
    partComboBox->addItem("xc7k325tffg900-2");

    //Create the layout
    fpgaDataGroupLayout = new QGridLayout();
    fpgaDataGroupLayout->addWidget(partLabel, 0, 0);
    fpgaDataGroupLayout->addWidget(partComboBox, 0, 1);
    fpgaDataGroupLayout->addWidget(boardLabel, 1, 0);
    fpgaDataGroupLayout->addWidget(boardComboBox, 1, 1);
    fpgaDataGroup->setLayout(fpgaDataGroupLayout);
}

void GeneralInfoWidget::createProjectDataGroup() {
    projectDataGroup = new QGroupBox(tr("Datos del proyecto (Vivado)"));

    //Create objects for group box
    //projectNameLabel = new QLabel(tr("Nombre del proyecto:"));
    designNameLabel = new QLabel(tr("Block design name:"));
    ipDirLabel = new QLabel(tr("Directorio de IPs del usuario:"));
    blockDesignLabel = new QLabel(tr("Archivo block design:"));
    //projectNameLineEdit = new QLineEdit();
    designNameLineEdit = new QLineEdit();
    ipDirLineEdit = new QLineEdit();
    blockDesignLineEdit = new QLineEdit();
    ipDirButton = new QPushButton(projectDataGroup);
    ipDirButton->setIcon(QIcon::fromTheme("folder"));
    ipDirButton->setFixedHeight(30);
    ipDirButton->setFixedWidth(30);
    ipDirButton->setToolTip(tr("Seleccionar el directorio de IPs"));
    blockDesignButton = new QPushButton(projectDataGroup);
    blockDesignButton->setIcon(QIcon::fromTheme("folder"));
    blockDesignButton->setFixedHeight(30);
    blockDesignButton->setFixedWidth(30);
    blockDesignButton->setToolTip(tr("Seleccionar el archivo que contiene el diseño de bloques"));

    //Connect button signals to the slots
    connect(ipDirButton, SIGNAL(clicked()), this, SLOT(selectIPDir()));
    connect(blockDesignButton, SIGNAL(clicked()), this, SLOT(selectBlockDesignFile()));

    //Create layout
    
    projectDataGroupLayout = new QGridLayout();
    /*
    projectDataGroupLayout->addWidget(projectNameLabel, 0, 0);
    projectDataGroupLayout->addWidget(projectNameLineEdit, 0, 1, 1, 2);
    projectDataGroupLayout->addWidget(designNameLabel, 1, 0);
    projectDataGroupLayout->addWidget(designNameLineEdit, 1, 1, 1, 2);
    projectDataGroupLayout->addWidget(ipDirLabel, 2, 0);
    projectDataGroupLayout->addWidget(ipDirLineEdit, 2, 1);
    projectDataGroupLayout->addWidget(ipDirButton, 2, 2);
    projectDataGroupLayout->addWidget(blockDesignLabel, 3, 0);
    projectDataGroupLayout->addWidget(blockDesignLineEdit, 3, 1);
    projectDataGroupLayout->addWidget(blockDesignButton, 3, 2);
    projectDataGroup->setLayout(projectDataGroupLayout);
    */
    projectDataGroupLayout->addWidget(designNameLabel, 0, 0);
    projectDataGroupLayout->addWidget(designNameLineEdit, 0, 1, 1, 2);
    projectDataGroupLayout->addWidget(ipDirLabel, 2, 0);
    projectDataGroupLayout->addWidget(ipDirLineEdit, 2, 1);
    projectDataGroupLayout->addWidget(ipDirButton, 2, 2);
    projectDataGroupLayout->addWidget(blockDesignLabel, 1, 0);
    projectDataGroupLayout->addWidget(blockDesignLineEdit, 1, 1);
    projectDataGroupLayout->addWidget(blockDesignButton, 1, 2);
    projectDataGroup->setLayout(projectDataGroupLayout);
}

void GeneralInfoWidget::listCompatibleBoards(const QString &part) {
    boardComboBox->clear();

    // FIXME: It must read this info from devices dir
    //Add boards to comboBox according to the part introduced
    if (part == "xc7z020clg484-1") {
#if defined VERSION==2015
        boardComboBox->addItem("em.avnet.com:zed:part0:1.2");
#else
        boardComboBox->addItem("em.avnet.com:zed:part0:1.3");
#endif
    }

    if (part == "xc7k325tffg900-2") {
#if defined VERSION==2015
        boardComboBox->addItem("xilinx.com:kc705:part0:1.2");
#else
        boardComboBox->addItem("xilinx.com:kc705:part0:1.3");
#endif
    }
}

void GeneralInfoWidget::selectIPDir() {
    QString dir;

    dir = QFileDialog::getExistingDirectory(this, tr("Seleccionar repositorio de IPs"), mainDir, QFileDialog::ShowDirsOnly);

    if (dir.size() > 0) {
        dir = dir.remove(mainDir);
        ipDirLineEdit->setText(dir);
    }
}

void GeneralInfoWidget::selectBlockDesignFile() {
    QString file;

    file = QFileDialog::getOpenFileName(this, tr("Seleccionar el archivo que contiene el block design"), mainDir, "TCL (*.tcl);; All files (*.*)");

    if (file.size() > 0) {
        file = file.remove(mainDir);
        blockDesignLineEdit->setText(file);
    }
}
