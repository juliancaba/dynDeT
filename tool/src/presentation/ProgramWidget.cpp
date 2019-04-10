#include "ProgramWidget.h"
#include "ProgramFPGA.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardItemModel>
#include <QMessageBox>
#include <iostream>

#define BITSTREAMS_FOLDER "Bitstreams/"
#define MAIN_FULL_BITSTREAM "reference.bit"
#define SDK_FOLDER "sdk/"


ProgramWidget::ProgramWidget(QWidget *parent, QString projectDir, string bdname) : QWidget(parent), mainDir(projectDir), blockDesign(bdname), isFPGAProgrammed(false), programmingNow(false) {
    //Label
    bitstreamsLabel = new QLabel(tr("Bitstreams:"));
    bitstreamsLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    //ComboBox
    createBitstreamCBox();

    //Program button
    programButton = new QPushButton(tr("Programar"), this);
    programButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(programButton, SIGNAL(clicked()), this, SLOT(program()));

    //Progress bar
    programProgressBar = new QProgressBar(this);
    programProgressBar->setVisible(false);

    //Check the number of available bitstreams
    if (bitstreamsCBox->count() == 1) {
        bitstreamsCBox->setEnabled(false);
        programButton->setEnabled(false);
    }

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(bitstreamsLabel, 0, 0);
    layout->addWidget(bitstreamsCBox, 0, 1, 1, 3);
    layout->addWidget(programProgressBar, 1, 0, 1, 3);
    layout->addWidget(programButton, 1, 3);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);

    //Disable focus for all buttons
    disableFocusOfElements();
}

void ProgramWidget::createBitstreamCBox() {
    QDir bitstreamsDir(mainDir + BITSTREAMS_FOLDER);
    QFileInfoList bitstreamsList;
    QString fileName;
    //int lastItemIndex;
    bool cont;

    bitstreamsCBox = new QComboBox();

    //List the directory
    bitstreamsList = bitstreamsDir.entryInfoList(QDir::Files, QDir::Name);

    //Add full bitstreams to combo box
    for (QFileInfoList::size_type i = 0; i < bitstreamsList.size(); i++) {
        fileName = bitstreamsList[i].fileName();

        if (fileName.endsWith(".bit") && !fileName.contains("pblock")) {
            bitstreamsCBox->addItem(fileName);
        }
    }

    //Add partial bitstreams to combo box (disabled)
    bitstreamsCBox->insertSeparator(bitstreamsCBox->count());
    //lastItemIndex = bitstreamsCBox->count() - 1;

    for (QFileInfoList::size_type i = 0; i < bitstreamsList.size(); i++) {
        fileName = bitstreamsList[i].fileName();

        if (fileName.endsWith(".bit") && fileName.contains("pblock")) {
            bitstreamsCBox->addItem(fileName);
            //lastItemIndex++;
            //qobject_cast<QStandardItemModel *>(bitstreamsCBox->model())->item(lastItemIndex)->setEnabled(false);
        }
    }

    //Select the main full bitstream
    cont = true;
    for (int i = 0; i < bitstreamsCBox->count() && cont; i++) {
        if (bitstreamsCBox->itemText(i) == MAIN_FULL_BITSTREAM) {
            bitstreamsCBox->setCurrentIndex(i);
            cont = false;
        }
    }
}

void ProgramWidget::disableFocusOfElements() {
    programButton->setFocusPolicy(Qt::NoFocus);
}

void ProgramWidget::program() {
  std::string outputFileName, deployTCLFileName, deployMKFileName;
    bool programTCLOK, deployTCLOK, deployMKOK;

    outputFileName = mainDir.toStdString() + "program.tcl";
#if defined VERSION==2015
    deployTCLFileName = mainDir.toStdString() + "xmd.tcl";
#else
    deployTCLFileName = mainDir.toStdString() + "xsdb.tcl";
#endif
    deployMKFileName = mainDir.toStdString() + "deploy.mk";

    bitstreamsCBox->setEnabled(false);
    programButton->setEnabled(false);

    //Show progress bar
    programProgressBar->setMinimum(0);
    programProgressBar->setMaximum(0);
    programProgressBar->setFixedHeight(15);
    programProgressBar->setVisible(true);

    //Generate program.tcl
    ProgramFPGA progFPGA(BITSTREAMS_FOLDER);
    progFPGA.setFullBitstream(bitstreamsCBox->currentText().toStdString());
    progFPGA.setWrapperHWPlatform("tmp/tmp.srcs/sources_1/bd/" + blockDesign + "/ip/" + blockDesign + "_processing_system7_0_0/ps7_init.tcl");

    programTCLOK = progFPGA.generateProgramTCL(outputFileName);
    deployTCLOK = progFPGA.generateSmallDeployTCL(deployTCLFileName);
    deployMKOK = progFPGA.generateSmallDeployMK(deployMKFileName);

    if (programTCLOK && deployTCLOK && deployMKOK) {
        //Launch the program process
        programmingNow = true;
        programProcess = new QProcess(this);
        programProcess->setWorkingDirectory(mainDir);
        connect(programProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(programProcessFinished(int,QProcess::ExitStatus)));
        programProcess->start("make -f deploy.mk program deploy");
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Error al generar el archivo necesario para programar la FPGA.") + "\n\n" + tr("Archivo:") + " " + QString::fromStdString(outputFileName));

        programProgressBar->setVisible(false);
        bitstreamsCBox->setEnabled(true);
        programButton->setEnabled(true);
    }
}

void ProgramWidget::programProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    QStandardItemModel *bitstreamsItemModel;

    programProgressBar->setMinimum(0);
    programProgressBar->setMaximum(100);
    programProgressBar->setValue(100);

    if (exitCode == 0 && exitStatus == 0) {
        QMessageBox::information(this, tr("Informacion"), tr("FPGA programada correctamente."));

        //If it is the first time the FPGA is programmed, enable the partial bitstreams
        if (!isFPGAProgrammed) {
            isFPGAProgrammed = true;

            bitstreamsItemModel = qobject_cast<QStandardItemModel *>(bitstreamsCBox->model());
            for (int i = 0; i < bitstreamsCBox->count(); i++) {
                if (!bitstreamsItemModel->item(i)->isEnabled()) {
                    bitstreamsItemModel->item(i)->setEnabled(true);
                }
            }
        }
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Ha ocurrido un error al programar la FPGA.") + "\n\n" + tr("Código de error:") + " " + QString::number(exitCode) + "\n\n" + tr("Errores:") + "\n" + programProcess->readAllStandardError());
    }

    //Hide progress bar
    programProgressBar->setVisible(false);

    programmingNow = false;
    bitstreamsCBox->setEnabled(true);
    programButton->setEnabled(true);
}

bool ProgramWidget::getProgrammingNow() {
    return programmingNow;
}
