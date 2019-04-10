#include "ReconfigurationManagement.h"
#include "ProgramWidget.h"
#include "ProgramUserAppWidget.h"


ReconfigurationManagement::ReconfigurationManagement(QWidget *parent, QString projectDir, QString blockDesign, string bdname) : QDialog(parent), mainDir(projectDir), blockDesignTCLFile(blockDesign), bdname(bdname) ,selectedOptionWidget(NULL) {
    QFont font;

    //Label
    titleLabel = new QLabel(tr("Gestión de la reconfiguración"));
    font = titleLabel->font();
    font.setPointSize(11);
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    //Create the option selector
    optionsLabel = new QLabel(tr("¿Cómo desea gestionar la reconfiguración de la FPGA?"));
    programUsingThisToolButton = new QRadioButton(tr("Utilizando esta herramienta"), this);
    programUsingThisToolButton->setChecked(true);
    programUserAppButton = new QRadioButton(tr("Utilizando una aplicación creada por el usuario"), this);
    bttnGroup = new QButtonGroup(this);
    bttnGroup->addButton(programUsingThisToolButton);
    bttnGroup->addButton(programUserAppButton);
    bttnGroup->setExclusive(true);
    nextButton = new QPushButton(tr("Siguiente"));
    nextButton->setFocusPolicy(Qt::NoFocus);
    connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));

    //Create the spacer
    spacer = new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(titleLabel, 0, 0);
    layout->addWidget(optionsLabel, 1, 0, 1, 2);
    layout->addWidget(programUsingThisToolButton, 2, 0, 1, 2);
    layout->addWidget(programUserAppButton, 3, 0, 1, 2);
    layout->addItem(spacer, 4, 0, 1, 2);
    layout->addWidget(nextButton, 5, 1, 1, 1, Qt::AlignRight);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);

    //Window
    setWindowTitle(tr("Gestión de la reconfiguración - dynDeT: dynamic partial reconfiguration DEsign Tool"));
    resize(600, 500);
}

void ReconfigurationManagement::next() {
    if (programUsingThisToolButton->isChecked()) {
        selectedOptionWidget = new ProgramWidget(this, mainDir, bdname);
    }
    if (programUserAppButton->isChecked()) {
        selectedOptionWidget = new ProgramUserAppWidget(this, mainDir, blockDesignTCLFile);
    }

    if (selectedOptionWidget != NULL) {
        //Remove the option selector
        layout->removeWidget(optionsLabel);
        layout->removeWidget(programUsingThisToolButton);
        layout->removeWidget(programUserAppButton);
        layout->removeWidget(nextButton);
        delete optionsLabel;
        delete programUsingThisToolButton;
        delete programUserAppButton;
        delete bttnGroup;
        delete nextButton;

        layout->addWidget(selectedOptionWidget, 1, 0, 1, 2);
    }
}

void ReconfigurationManagement::closeEvent(QCloseEvent *event) {
    ReconfManagementOptionInterface *selectedOptionIface;

    selectedOptionIface = dynamic_cast<ReconfManagementOptionInterface *>(selectedOptionWidget);

    if (selectedOptionIface == NULL) {
        event->accept();
    }
    else {
        if (selectedOptionIface->getProgrammingNow()) {
            event->ignore();
        }
        else {
            event->accept();
        }
    }

}

void ReconfigurationManagement::keyPressEvent(QKeyEvent *event) {
    ReconfManagementOptionInterface *selectedOptionIface;

    if (event->key() == Qt::Key_Escape) {
        selectedOptionIface = dynamic_cast<ReconfManagementOptionInterface *>(selectedOptionWidget);

        if (selectedOptionIface == NULL) {
            QDialog::keyPressEvent(event);
        }
        else {
            if (!selectedOptionIface->getProgrammingNow()) {
                QDialog::keyPressEvent(event);
            }
        }
    }
    else {
        QDialog::keyPressEvent(event);
    }
}
