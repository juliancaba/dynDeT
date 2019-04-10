#include "StaticPartWidget.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QCheckBox>

#include <iostream>
using namespace std;

StaticPartWidget::StaticPartWidget(QWidget *parent, Design *des) : QWidget(parent), mainDesign(des) {
    createStaticFilesGroup();
    createConstraintFilesGroup();

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(staticFilesGroup, 0, 0);
    layout->addWidget(constraintFilesGroup, 1, 0);
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);
}

void StaticPartWidget::setProjectDir(QString &str) {
    mainDir = str;
}

void StaticPartWidget::enableAllEditElements() {
    int rows;
    QCheckBox *topColumnCheckBox;

    addStaticFileButton->setEnabled(true);
    removeStaticFileButton->setEnabled(true);
    addConstraintFileButton->setEnabled(true);
    removeConstraintFileButton->setEnabled(true);

    //Check the table of static files
    rows = staticFilesTable->rowCount();
    for (int i = 0; i < rows; i++) {
        topColumnCheckBox = qobject_cast<QCheckBox *>(staticFilesTable->cellWidget(i, 0));
        topColumnCheckBox->setEnabled(true);
    }
    staticFilesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    constraintFilesList->setSelectionMode(QAbstractItemView::SingleSelection);
}

void StaticPartWidget::disableAllEditElements() {
    int rows;
    QCheckBox *topColumnCheckBox;

    addStaticFileButton->setEnabled(false);
    removeStaticFileButton->setEnabled(false);
    addConstraintFileButton->setEnabled(false);
    removeConstraintFileButton->setEnabled(false);

    //Check the table of static files
    rows = staticFilesTable->rowCount();
    for (int i = 0; i < rows; i++) {
        topColumnCheckBox = qobject_cast<QCheckBox *>(staticFilesTable->cellWidget(i, 0));
        topColumnCheckBox->setEnabled(false);
    }
    staticFilesTable->clearSelection();
    staticFilesTable->setSelectionMode(QAbstractItemView::NoSelection);
    constraintFilesList->clearSelection();
    constraintFilesList->setSelectionMode(QAbstractItemView::NoSelection);
}

void StaticPartWidget::addToDesign() {
    int rowsStaticFilesTable;
    QString topFile, constraintFile, auxFilePath, auxFileName;
    QStringList otherStaticFiles;
    QCheckBox *topColumnCheckBox;
    QTableWidgetItem *fileColumnItem;

    //Add static files to design
    rowsStaticFilesTable = staticFilesTable->rowCount();
    for (int i = 0; i < rowsStaticFilesTable; i++) {
        topColumnCheckBox = qobject_cast<QCheckBox *>(staticFilesTable->cellWidget(i, 0));
        fileColumnItem = staticFilesTable->item(i, 1);

        if (topColumnCheckBox->isChecked()) {
            topFile = fileColumnItem->text();
        }
        else {
            otherStaticFiles.append(fileColumnItem->text());
        }
    }

    if (rowsStaticFilesTable != 0){
      //The first static file added to the design has to be the TOP file
      auxFilePath = getPathOfFile(topFile);
      auxFileName = getNameOfFile(topFile);
      mainDesign->addStaticFile(auxFilePath.toStdString(), auxFileName.toStdString());
    }

    //Add the other static files
    for (QStringList::size_type i = 0; i < otherStaticFiles.size(); i++) {
        auxFilePath = getPathOfFile(otherStaticFiles[i]);
        auxFileName = getNameOfFile(otherStaticFiles[i]);
        mainDesign->addStaticFile(auxFilePath.toStdString(), auxFileName.toStdString());
    }

    //Add constraint files to design
    for (int i = 0; i < constraintFilesList->count(); i++) {
        constraintFile = constraintFilesList->item(i)->text();
        auxFilePath = getPathOfFile(constraintFile);
        auxFileName = getNameOfFile(constraintFile);
        mainDesign->addConstraintFile(auxFilePath.toStdString(), auxFileName.toStdString());
    }
}

void StaticPartWidget::loadFromDesign() {
    std::vector<std::vector<std::string> > staticFiles, constraintFiles;
    QCheckBox *isTopCheckBox;
    QTableWidgetItem *file;
    QString fileStr;
    int newRowIndex;

    //Add static files to the table
    staticFiles = mainDesign->getStaticFiles();

    if (staticFiles.size() != 0){
      for (size_t i = 0; i < staticFiles.size(); i++) {
        isTopCheckBox = new QCheckBox();
        if (i == 0) {
	  isTopCheckBox->setChecked(true);
        }
        else {
	  isTopCheckBox->setChecked(false);
        }
        topCheckBoxesGroup->addButton(isTopCheckBox);

        fileStr = QString::fromStdString(staticFiles[i][0]) + QString::fromStdString(staticFiles[i][1]);
        file = new QTableWidgetItem(fileStr);

        newRowIndex = staticFilesTable->rowCount();
        staticFilesTable->insertRow(newRowIndex);
        staticFilesTable->setCellWidget(newRowIndex, 0, isTopCheckBox);
        staticFilesTable->setItem(newRowIndex, 1, file);
      }
    }

    //Add constraint files to the list
    constraintFiles = mainDesign->getConstraintFiles();

    for (size_t i = 0; i < constraintFiles.size(); i++) {
        fileStr = QString::fromStdString(constraintFiles[i][0]) + QString::fromStdString(constraintFiles[i][1]);
        constraintFilesList->addItem(fileStr);
    }
}

bool StaticPartWidget::check() {
    int rows;
    bool tableOK = false, listOK = false;
    QCheckBox *topColumnCheckBox;

    //Check the table of static files
    rows = staticFilesTable->rowCount();
    if (rows > 0) {
        for (int i = 0; i < rows && !tableOK; i++) {
            topColumnCheckBox = qobject_cast<QCheckBox *>(staticFilesTable->cellWidget(i, 0));

            if (topColumnCheckBox->isChecked()) {
                tableOK = true;
            }
        }
    }
    else
      tableOK = true;

    if (tableOK) {
        staticFilesTable->setStyleSheet("");
    }
    else {
        staticFilesTable->setStyleSheet("background: #ffcccc");
    }

    //Check the list of constraint files
    if (constraintFilesList->count() > 0) {
        constraintFilesList->setStyleSheet("");
        listOK = true;
    }
    listOK = true;
    // else {
    //     constraintFilesList->setStyleSheet("background: #ffcccc");
    // }


    return tableOK && listOK;
}


void StaticPartWidget::createStaticFilesGroup() {
    QStringList tableHeader;

    staticFilesGroup = new QGroupBox(tr("Otros archivos de la parte estática"));

    //Create the table that shows the list of static files (and the "top" checkbox).
    tableHeader.append(tr("Top"));
    tableHeader.append(tr("Ruta"));
    staticFilesTable = new QTableWidget(staticFilesGroup);
    staticFilesTable->setColumnCount(2);
    staticFilesTable->setColumnWidth(0, 30);
    staticFilesTable->setHorizontalHeaderLabels(tableHeader);
    staticFilesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    staticFilesTable->horizontalHeader()->setStretchLastSection(true);
    staticFilesTable->verticalHeader()->hide();
    staticFilesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    staticFilesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    staticFilesTable->setSelectionMode(QAbstractItemView::SingleSelection);

    //Create the add and remove buttons
    addStaticFileButton = new QPushButton(tr("Añadir"), staticFilesGroup);
    addStaticFileButton->setIcon(QIcon::fromTheme("list-add"));
    addStaticFileButton->setFixedHeight(30);
    addStaticFileButton->setFixedWidth(85);
    removeStaticFileButton = new QPushButton(tr("Eliminar"), staticFilesGroup);
    removeStaticFileButton->setIcon(QIcon::fromTheme("list-remove"));
    removeStaticFileButton->setFixedHeight(30);
    removeStaticFileButton->setFixedWidth(85);

    //Connect button signals to the slots
    connect(addStaticFileButton, SIGNAL(clicked()), this, SLOT(addStaticFiles()));
    connect(removeStaticFileButton, SIGNAL(clicked()), this, SLOT(removeStaticFile()));

    //Button group
    topCheckBoxesGroup = new QButtonGroup(staticFilesGroup);
    topCheckBoxesGroup->setExclusive(true);

    //Create the layout for the groupbox
    staticFilesGroupLayout = new QGridLayout();
    staticFilesGroupLayout->addWidget(staticFilesTable, 0, 0, 1, 3);
    staticFilesGroupLayout->addWidget(addStaticFileButton, 1, 1);
    staticFilesGroupLayout->addWidget(removeStaticFileButton, 1, 2);
    staticFilesGroup->setLayout(staticFilesGroupLayout);
}

void StaticPartWidget::createConstraintFilesGroup() {
    constraintFilesGroup = new QGroupBox(tr("Archivos de restricciones (constraint files)"));

    //Create the list that shows the constraint files
    constraintFilesList = new QListWidget(constraintFilesGroup);


    //Create the add and remove buttons
    addConstraintFileButton = new QPushButton(tr("Añadir"), constraintFilesGroup);
    addConstraintFileButton->setIcon(QIcon::fromTheme("list-add"));
    addConstraintFileButton->setFixedHeight(30);
    addConstraintFileButton->setFixedWidth(85);
    removeConstraintFileButton = new QPushButton(tr("Eliminar"), constraintFilesGroup);
    removeConstraintFileButton->setIcon(QIcon::fromTheme("list-remove"));
    removeConstraintFileButton->setFixedHeight(30);
    removeConstraintFileButton->setFixedWidth(85);

    //Connect buttons signals to the slots
    connect(addConstraintFileButton, SIGNAL(clicked()), this, SLOT(addConstraintFiles()));
    connect(removeConstraintFileButton, SIGNAL(clicked()), this, SLOT(removeConstraintFile()));


    //Create the layout for the groupbox
    constraintFilesGroupLayout = new QGridLayout();
    constraintFilesGroupLayout->addWidget(constraintFilesList, 0, 0, 1, 3);
    constraintFilesGroupLayout->addWidget(addConstraintFileButton, 1, 1);
    constraintFilesGroupLayout->addWidget(removeConstraintFileButton, 1, 2);
    constraintFilesGroup->setLayout(constraintFilesGroupLayout);
}

QString StaticPartWidget::getPathOfFile(QString &file) {
    int index;
    QString res;

    index = file.lastIndexOf("/");
    res = file.left(index + 1);

    return res;
}

QString StaticPartWidget::getNameOfFile(QString &file) {
    int index;
    QString res;

    index = file.lastIndexOf("/");
    res = file.right(file.size() - index - 1);

    return res;
}

void StaticPartWidget::addStaticFiles() {
    QStringList files;

    //Open QFileDialog
    files = QFileDialog::getOpenFileNames(this, tr("Seleccionar los archivos de la parte estática"), mainDir, "All files (*.*)");

    //Add each file to the table
    for (QStringList::size_type i = 0; i < files.size(); i++) {
        int newRowIndex;
        QCheckBox *isTopCheckBox;
        QTableWidgetItem *filePath;

        isTopCheckBox = new QCheckBox();
        isTopCheckBox->setChecked(false);
        topCheckBoxesGroup->addButton(isTopCheckBox);

        filePath = new QTableWidgetItem(files[i].remove(mainDir));

        newRowIndex = staticFilesTable->rowCount();
        staticFilesTable->insertRow(newRowIndex);
        staticFilesTable->setCellWidget(newRowIndex, 0, isTopCheckBox);
        staticFilesTable->setItem(newRowIndex, 1, filePath);
    }
}

void StaticPartWidget::removeStaticFile() {
    int selectedRow;

    //Remove the selected row from the table of static files
    selectedRow = staticFilesTable->currentRow();
    staticFilesTable->removeRow(selectedRow);
}

void StaticPartWidget::addConstraintFiles() {
    QStringList files;

    //Open QFileDialog
    files = QFileDialog::getOpenFileNames(this, tr("Seleccionar los archivos de restricciones"), mainDir, "Xilinx Design Constraints (*.xdc);; All files (*.*)");

    //Add each file to the list
    for (QStringList::size_type i = 0; i < files.size(); i++) {
        constraintFilesList->addItem(files[i].remove(mainDir));
    }
}

void StaticPartWidget::removeConstraintFile() {
    QList<QListWidgetItem *> items;

    items = constraintFilesList->selectedItems();

    //Remove the selected files from the list of constraint files.
    for (QList<QListWidgetItem *>::size_type i = 0; i < items.size(); i++) {
        delete constraintFilesList->takeItem(constraintFilesList->row(items[i]));
    }
}
