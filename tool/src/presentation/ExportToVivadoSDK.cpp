#include "ExportToVivadoSDK.h"
#include <QMessageBox>

ExportToVivadoSDK::ExportToVivadoSDK(QWidget *parent, QString projectDir) : QDialog(parent), mainDir(projectDir), exportingNow(false) {
    QFont font;

    //Label
    titleLabel = new QLabel(tr("Exportar hardware a Vivado SDK"));
    font = titleLabel->font();
    font.setPointSize(11);
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    //Create the button group (check boxes) - Options
    // addBitstreamOption = new QCheckBox(tr("Añadir el bitstream de referencia (reference.bit)"), this);
    // addBitstreamOption->setChecked(true);
    launchSDKOption = new QCheckBox(tr("Abrir Vivado SDK al finalizar"), this);
    bttnGroup = new QButtonGroup(this);
    //bttnGroup->addButton(addBitstreamOption);
    bttnGroup->addButton(launchSDKOption);
    bttnGroup->setExclusive(false);

    //Export button
    exportButton = new QPushButton(tr("Exportar"), this);
    exportButton->setFocusPolicy(Qt::NoFocus);
    connect(exportButton, SIGNAL(clicked()), this, SLOT(exportToSDK()));

    //Progress bar
    exportProgressBar = new QProgressBar(this);
    exportProgressBar->setVisible(false);

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(titleLabel, 0, 0);
    //layout->addWidget(addBitstreamOption, 1, 0, 1, 2);
    layout->addWidget(launchSDKOption, 2, 0, 1, 2);
    layout->addWidget(exportProgressBar, 3, 0);
    layout->addWidget(exportButton, 3, 1, 1, 1, Qt::AlignRight);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);

    //Window
    setWindowTitle(tr("Exportar hardware a Vivado SDK - dynDeT: dynamic partial reconfiguration DEsign Tool"));
    resize(400, 125);
}

void ExportToVivadoSDK::exportToSDK() {
    QString command;

    //addBitstreamOption->setEnabled(false);
    launchSDKOption->setEnabled(false);
    exportButton->setEnabled(false);

    //Check options
    command = "make runSDKExportTCL";

    // if (addBitstreamOption->isChecked()) {
    //     command += " runAddBitstreamTCL";
    // }
    if (launchSDKOption->isChecked()) {
        command += " runSDKLaunchTCL";
    }

    //Show progress bar
    exportProgressBar->setMinimum(0);
    exportProgressBar->setMaximum(0);
    exportProgressBar->setFixedHeight(15);
    exportProgressBar->setVisible(true);

    //Launch the export process
    exportingNow = true;
    exportProcess = new QProcess(this);
    exportProcess->setWorkingDirectory(mainDir);
    connect(exportProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(exportProcessFinished(int,QProcess::ExitStatus)));
    exportProcess->start(command);
}

void ExportToVivadoSDK::exportProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    exportProgressBar->setMinimum(0);
    exportProgressBar->setMaximum(100);
    exportProgressBar->setValue(100);

    if (exitCode == 0 && exitStatus == 0) {
        QMessageBox::information(this, tr("Informacion"), tr("Hardware exportado correctamente."));
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Error al exportar el hardware a Vivado SDK.") + "\n\n" + tr("Código de error:") + " " + QString::number(exitCode) + "\n\n" + tr("Errores:") + "\n" + exportProcess->readAllStandardError());
    }

    //Hide progress bar
    exportProgressBar->setVisible(false);

    exportingNow = false;
    //addBitstreamOption->setEnabled(true);
    launchSDKOption->setEnabled(true);
    exportButton->setEnabled(true);
}


void ExportToVivadoSDK::closeEvent(QCloseEvent *event) {
    if (exportingNow) {
        event->ignore();
    }
    else {
        event->accept();
    }
}

void ExportToVivadoSDK::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        if (!exportingNow) {
            QDialog::keyPressEvent(event);
        }
    }
    else {
        QDialog::keyPressEvent(event);
    }
}
