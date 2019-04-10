#include "FloorplanWidget.h"
#include <QHeaderView>
#include <QFile>
#include <QMessageBox>
#include <QMenu>

#define XML_DIR "/usr/share/dynDeT/devices/"
#define MINIMUM_ZOOM 3
#define MAXIMUM_ZOOM 18
#define ZOOM_VARIATION 3


FloorplanWidget::FloorplanWidget(QWidget *parent) : QDialog(parent), boardIsLoaded(false) {
    QFont font;

    createBoardGroup();
    createToolsGroup();
    createSearchSiteGroup();
    createHWResourcesGroup();

    //Label
    titleLabel = new QLabel(tr("Selector de área"));
    font = titleLabel->font();
    font.setPointSize(12);
    font.setBold(true);
    titleLabel->setFont(font);
    titleLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    //Lines
    hLineBottom = new QFrame();
    hLineBottom->setFrameShape(QFrame::HLine);
    hLineBottom->setFrameShadow(QFrame::Sunken);

    //Accept button
    acceptButton = new QPushButton(tr("Aceptar"), this);
    acceptButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));

    //Set window
    resize(1000, 650);
    setWindowTitle(tr("Selector de área - dynDeT: dynamic partial reconfiguration DEsign Tool"));

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(titleLabel, 0, 0, 1, 2);
    layout->addWidget(boardGroup, 1, 0, 3, 1);
    layout->addWidget(toolsGroup, 1, 1);
    layout->addWidget(searchSiteGroup, 2, 1);
    layout->addWidget(hwResourcesGroup, 3, 1);
    layout->addWidget(hLineBottom, 4, 0, 1, 2);
    layout->addWidget(acceptButton, 5, 1);
    layout->setAlignment(acceptButton, Qt::AlignRight);
    setLayout(layout);

    //Disable focus for all buttons and the check box
    disableFocusOfElements();
}

bool FloorplanWidget::loadBoardData(QString boardID) {
    QString fileName, hwResourceTooltip;
    QTableWidgetItem *hwResource;
    int rowIndex, columnIndex;
    bool loadFromXMLOK;

    fileName = XML_DIR + boardID + ".xml";

    loadFromXMLOK = board.loadFromXML(fileName.toStdString());

    if (!loadFromXMLOK) {
        QMessageBox::critical(this, tr("Error"), tr("Error al leer el archivo") + " " + fileName);
    }
    else {
        //Set boardGroup and boardTable
        boardGroup->setTitle("Board (" + QString::fromStdString(board.getName()) + ")");
        boardTable->setRowCount(board.getRows());
        boardTable->setColumnCount(board.getCols());

        //Add sites to boardTable
        for (int i = 0; i < board.getNumOfSites(); i++) {
            Site site(board.getSiteAt(i));

            //row = numRows - RPM_Y - 1; column = RPM_X
            rowIndex = board.getRows() - site.getRPMY() - 1;
            columnIndex = site.getRPMX();

            //It the site is RAMB32, span it!
            if (site.getType() == "RAMBFIFO36E1") {
                rowIndex -= 1;
                boardTable->setSpan(rowIndex, columnIndex, 2, 1);
            }

            //Get the table widget item
            hwResource = boardTable->item(rowIndex, columnIndex);

            if (hwResource == 0) {
                hwResource = new QTableWidgetItem();
                boardTable->setItem(rowIndex, columnIndex, hwResource);
            }

            //Set the background color of the item
            QBrush brush(Qt::black);
            selectSiteColor(brush, QString::fromStdString(site.getType()));
            hwResource->setBackground(brush);

            //Make the tooltip
            hwResourceTooltip = QString::fromStdString(site.getName()) + "\n" + tr("Tipo:") + " " + QString::fromStdString(site.getType()) + "\n" + tr("Región de reloj:") + " " + QString::fromStdString(site.getClockRegion());
            hwResource->setToolTip(hwResourceTooltip);
        }

        //Fill the empty cells of the table with a blank item
        fillEmptyCells();

        boardIsLoaded = true;
    }

    return loadFromXMLOK;
}

QString FloorplanWidget::getHWResources() {
    QTreeWidgetItem *topItem, *item;
    QString hwRes;

    for (int i = 0; i < hwResourcesTree->topLevelItemCount(); i++) {
        topItem = hwResourcesTree->topLevelItem(i);
        item = topItem->child(0);
        hwRes += item->text(0) + " ";
    }

    return hwRes;
}

void FloorplanWidget::disableAddedHWResources() {
    occupiedHWResources.append(lastAddedHWResources);

    for (QList<QTableWidgetItem *>::size_type i = 0; i < lastAddedHWResources.size(); i++) {
        lastAddedHWResources[i]->setFlags(Qt::NoItemFlags);
    }

    lastAddedHWResources.clear();
    hwResourcesTree->clear();
}

bool FloorplanWidget::getBoardIsLoaded() {
    return boardIsLoaded;
}

void FloorplanWidget::clearHWResData() {
    boardTable->clearSelection();
    lastAddedHWResources.clear();
    hwResourcesTree->clear();
}

void FloorplanWidget::removePartitionArea(int index) {
    if (index >= 0 && index < occupiedHWResources.size()) {
        for (QList<QTableWidgetItem *>::size_type i = 0; i < occupiedHWResources[index].size(); i++) {
            occupiedHWResources[index][i]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }
    }

    occupiedHWResources.removeAt(index);
}

void FloorplanWidget::createBoardGroup() {
    boardGroup = new QGroupBox(this);

    //Create the board table
    boardTable = new QTableWidget(this);
    boardTable->horizontalHeader()->setDefaultSectionSize(MINIMUM_ZOOM);
    boardTable->verticalHeader()->setDefaultSectionSize(MINIMUM_ZOOM);
    boardTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    boardTable->setSelectionMode(QAbstractItemView::ContiguousSelection);
    boardTable->setShowGrid(false);
    boardTable->horizontalHeader()->hide();
    boardTable->verticalHeader()->hide();
    boardTable->setItemDelegate(new SelectionDelegate(boardTable));
    boardTable->setStyleSheet("gridline-color: #000000");

    //Create "add selected hw resources" button
    addSelectedHWResourcesButton = new QPushButton(tr("Añadir los recursos seleccionados"), boardGroup);
    addSelectedHWResourcesButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(addSelectedHWResourcesButton, SIGNAL(clicked()), this, SLOT(addSelectedHWResourcesToTreeWidget()));

    //Create the layout for the groupbox
    boardGroupLayout = new QGridLayout();
    boardGroupLayout->addWidget(boardTable, 0, 0, 1, 2);
    boardGroupLayout->addWidget(addSelectedHWResourcesButton, 1, 1);
    boardGroup->setLayout(boardGroupLayout);
}

void FloorplanWidget::createToolsGroup() {
    toolsGroup = new QGroupBox(tr("Herramientas de visualización"), this);

    //Zoom buttons
    zoomInButton = new QPushButton(tr("Zoom +"), toolsGroup);
    zoomOutButton = new QPushButton(tr("Zoom -"), toolsGroup);
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    //Create the layout for the groupbox
    toolsGroupLayout = new QGridLayout();
    toolsGroupLayout->addWidget(zoomOutButton, 0, 0);
    toolsGroupLayout->addWidget(zoomInButton, 0, 1);
    //toolsGroupLayout->addWidget(showGridCheckBox, 1, 0, 1, 2);
    toolsGroupLayout->setAlignment(Qt::AlignTop);
    toolsGroup->setLayout(toolsGroupLayout);
    toolsGroup->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void FloorplanWidget::createSearchSiteGroup() {
    searchSiteGroup = new QGroupBox(tr("Buscador"), this);

    //Create the lineEdit, label, and button
    searchSiteLineEdit = new QLineEdit();
    searchSiteButton = new QPushButton(tr("Buscar"), searchSiteGroup);
    searchSiteButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(searchSiteButton, SIGNAL(clicked()), this, SLOT(searchSite()));
    searchSiteResultLabel = new QLabel();
    searchSiteResultLabel->setVisible(false);

    //Create the layout for the groupbox
    searchSiteGroupLayout = new QGridLayout();
    searchSiteGroupLayout->addWidget(searchSiteLineEdit, 0, 0, 1, 2);
    searchSiteGroupLayout->addWidget(searchSiteResultLabel, 1, 0);
    searchSiteGroupLayout->addWidget(searchSiteButton, 1, 1);
    searchSiteGroup->setLayout(searchSiteGroupLayout);
}

void FloorplanWidget::createHWResourcesGroup() {
    hwResourcesGroup = new QGroupBox(tr("Recursos hardware añadidos"), this);

    //Create the tree widget
    hwResourcesTree = new QTreeWidget(hwResourcesGroup);
    hwResourcesTree->setSelectionMode(QAbstractItemView::SingleSelection);
    hwResourcesTree->header()->setStretchLastSection(false);
    hwResourcesTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    hwResourcesTree->header()->hide();
    hwResourcesTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(hwResourcesTree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showHWResourcesTreeWidgetContextMenu(QPoint)));
    hwResourcesTree->setFixedWidth(200);

    //Create the layout for the groupbox
    hwResourcesGroupLayout = new QGridLayout();
    hwResourcesGroupLayout->addWidget(hwResourcesTree, 0, 0);
    hwResourcesGroup->setLayout(hwResourcesGroupLayout);
    hwResourcesGroup->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
}

void FloorplanWidget::selectSiteColor(QBrush &brush, QString type) {
    if (type == "SLICEL" || type == "SLICEM") {
        //Blue
        brush.setColor(QColor(0, 0, 255));
    }

    if (type == "RAMB18E1" || type == "FIFO18E1" || type == "RAMBFIFO36E1") {
        //Red
        brush.setColor(QColor(255, 0, 0));
    }

    if (type == "DSP48E1") {
        //Dark green
        brush.setColor(QColor(0, 128, 0));
    }

    if (type == "MMCME2_ADV" || type == "PHASER_IN_PHY" || type == "PHASER_OUT_PHY" || type == "OUT_FIFO" || type == "IN_FIFO" || type == "PLLE2_ADV" || type == "PHY_CONTROL" || type == "PHASER_REF") {
        //Light orange
        brush.setColor(QColor(255, 204, 102));
    }

    if (type == "BUFIO" || type == "BUFR" || type == "IDELAYCTRL" || type == "BUFMRCE" || type == "BUFHCE" || type == "BUFGCTRL") {
        //Dark orange
        brush.setColor(QColor(255, 102, 0));
    }

    if (type == "OLOGICE2" || type == "OLOGICE3" || type == "ILOGICE2" || type == "ILOGICE3" || type == "IDELAYE2" || type == "ODELAYE2" || type == "IDELAYE2_FINEDELAY") {
        //Dark yellow
        brush.setColor(QColor(230, 230, 0));
    }

    if (type == "EFUSE_USR" || type == "DNA_PORT" || type == "ICAP" || type == "BSCAN" || type == "DCIRESET" || type == "CAPTURE" || type == "STARTUP" || type == "FRAME_ECC" || type == "USR_ACCESS" || type == "XADC" || type == "IOPAD" || type == "GTXE2_CHANNEL" || type == "GTXE2_COMMON" || type == "IBUFDS_GTE2") {
        //Gray
        brush.setColor(QColor(166, 166, 166));
    }

    if (type == "IPAD" || type == "OPAD") {
        //Pink
        brush.setColor(QColor(255, 77, 255));
    }

    if (type == "IOB18" || type == "IOB18M" || type == "IOB18S" || type == "IOB33" || type == "IOB33M" || type == "IOB33S") {
        //Light blue
        brush.setColor(QColor(0, 191, 255));
    }

    if (type == "PS7") {
        //Green
        brush.setColor(QColor(0, 204, 68));
    }

    if (type == "PCIE_2_1") {
        //Purple
        brush.setColor(QColor(95, 4, 180));
    }
}

void FloorplanWidget::fillEmptyCells() {
    QTableWidgetItem *item;
    QBrush brush(Qt::black);

    for (int i = 0; i < boardTable->rowCount(); i++) {
        for (int j = 0; j < boardTable->columnCount(); j++) {
            item = boardTable->item(i, j);

            if (item == 0) {
                item = new QTableWidgetItem();
                item->setBackground(brush);
                boardTable->setItem(i, j, item);
            }
        }
    }
}

void FloorplanWidget::addSelectedHWResourcesToTreeWidget() {
    QList<QList<QTableWidgetItem *> > clasifiedHWRes;
    QStringList hwResourcesCategories, hwResourcesShortStr;
    QString tooltip, hwResourceName, hwResourceCoordinates;

    hwResourcesTree->clear();
    lastAddedHWResources = boardTable->selectedItems();

    //Classify hw resources
    for (QList<QTableWidgetItem *>::size_type i = 0; i < lastAddedHWResources.size(); i++) {
        QString hwResourceCategory;
        int index;

        tooltip = lastAddedHWResources[i]->toolTip();

        if (!tooltip.isEmpty()) {
            hwResourceName = tooltip.left(tooltip.indexOf("\n"));
            hwResourceCategory = hwResourceName.left(hwResourceName.lastIndexOf("_"));

            if (!hwResourcesCategories.contains(hwResourceCategory)) {
                QList<QTableWidgetItem *> aux;

                hwResourcesCategories.append(hwResourceCategory);
                clasifiedHWRes.append(aux);
            }

            index = hwResourcesCategories.indexOf(hwResourceCategory);
            clasifiedHWRes[index].append(lastAddedHWResources[i]);
        }
    }

    //Get the TYPE_X??Y?? form for every hw resource category
    for (QStringList::size_type i = 0; i < hwResourcesCategories.size(); i++) {
        QString minimumStr, maximumStr;
        int x, y, minimumX = 0, maximumX = 0, minimumY = 0, maximumY = 0;

        tooltip = clasifiedHWRes[i][0]->toolTip();

        hwResourceName = tooltip.left(tooltip.indexOf("\n"));
        hwResourceCoordinates = hwResourceName.mid(hwResourceName.lastIndexOf("_") + 1);
        x = hwResourceCoordinates.left(hwResourceCoordinates.indexOf("Y")).mid(hwResourceCoordinates.indexOf("X") + 1).toInt();
        y = hwResourceCoordinates.mid(hwResourceCoordinates.indexOf("Y") + 1).toInt();

        minimumX = maximumX = x;
        minimumY = maximumY = y;

        if (clasifiedHWRes[i].size() > 1) {
            for (QList<QTableWidgetItem *>::size_type j = 1; j < clasifiedHWRes[i].size(); j++) {
                tooltip = clasifiedHWRes[i][j]->toolTip();

                hwResourceName = tooltip.left(tooltip.indexOf("\n"));
                hwResourceCoordinates = hwResourceName.mid(tooltip.lastIndexOf("_") + 1);
                x = hwResourceCoordinates.left(hwResourceCoordinates.indexOf("Y")).mid(hwResourceCoordinates.indexOf("X") + 1).toInt();
                y = hwResourceCoordinates.mid(hwResourceCoordinates.indexOf("Y") + 1).toInt();

                if (x <= minimumX && y <= minimumY) {
                    minimumX = x;
                    minimumY = y;
                }
                else {
                    if (x >= maximumX && y >= maximumY) {
                        maximumX = x;
                        maximumY = y;
                    }
                }
            }

            minimumStr = hwResourcesCategories[i] + "_X" + QString::number(minimumX) + "Y" + QString::number(minimumY);
            maximumStr = hwResourcesCategories[i] + "_X" + QString::number(maximumX) + "Y" + QString::number(maximumY);
            hwResourcesShortStr.append(minimumStr + ":" + maximumStr);
        }
        else {
            hwResourcesShortStr.append(hwResourcesCategories[i] + "_X" + QString::number(minimumX) + "Y" + QString::number(minimumY));
        }
    }

    //Fill the hw resources tree
    for (QStringList::size_type i = 0; i < hwResourcesCategories.size(); i++) {
        QTreeWidgetItem *categoryTopItem, *hwResShortItem;

        hwResShortItem = new QTreeWidgetItem();
        hwResShortItem->setText(0, hwResourcesShortStr[i]);
        hwResShortItem->setFlags(Qt::ItemIsEnabled);
        categoryTopItem = new QTreeWidgetItem();
        categoryTopItem->setText(0, hwResourcesCategories[i] + " (" + QString::number(clasifiedHWRes[i].size()) + ")");
        categoryTopItem->addChild(hwResShortItem);
        categoryTopItem->setExpanded(true);
        hwResourcesTree->addTopLevelItem(categoryTopItem);
    }
}

void FloorplanWidget::zoomIn() {
    int actualZoom;

    actualZoom = boardTable->horizontalHeader()->defaultSectionSize();

    if (actualZoom < MAXIMUM_ZOOM) {
        boardTable->horizontalHeader()->setDefaultSectionSize(actualZoom + ZOOM_VARIATION);
        boardTable->verticalHeader()->setDefaultSectionSize(actualZoom + ZOOM_VARIATION);
    }
}

void FloorplanWidget::zoomOut() {
    int actualZoom;

    actualZoom = boardTable->horizontalHeader()->defaultSectionSize();

    if (actualZoom > MINIMUM_ZOOM) {
        boardTable->horizontalHeader()->setDefaultSectionSize(actualZoom - ZOOM_VARIATION);
        boardTable->verticalHeader()->setDefaultSectionSize(actualZoom - ZOOM_VARIATION);
    }
}

void FloorplanWidget::showHWResourcesTreeWidgetContextMenu(const QPoint &pos) {
    QPoint globalPosition;
    QMenu menu;

    if (hwResourcesTree->selectedItems().size() > 0) {
        globalPosition = hwResourcesTree->mapToGlobal(pos);

        menu.addAction(tr("Eliminar"), this, SLOT(removeHWResourcesTreeWidgetSelectedItems()));
        menu.exec(globalPosition);
    }
}

void FloorplanWidget::removeHWResourcesTreeWidgetSelectedItems() {
    QList<QTreeWidgetItem *> itemsToRemove;

    itemsToRemove = hwResourcesTree->selectedItems();

    for (QList<QTreeWidgetItem *>::size_type i = 0; i < itemsToRemove.size(); i++) {
        delete itemsToRemove[i];
    }
}

void FloorplanWidget::searchSite() {
    bool found = false;
    QTableWidgetItem *item;
    QString siteName, hwResourceName, tooltip;

    siteName = searchSiteLineEdit->text();

    //Search the site
    for (int i = 0; i < boardTable->rowCount() && !found; i++) {
        for (int j = 0; j < boardTable->columnCount() && !found; j++) {
            item = boardTable->item(i, j);

            if (item != 0) {
                tooltip = item->toolTip();

                if (!tooltip.isEmpty()) {
                    hwResourceName = tooltip.left(tooltip.indexOf("\n"));

                    if (hwResourceName == siteName) {
                        found = true;

                        //If it was found, mark the site with an X. If not, show an error.
                        searchSiteResultLabel->setVisible(false);
                        item->setText("X");

                        boardTable->scrollToItem(item, QAbstractItemView::PositionAtCenter);
                    }
                }
            }
        }
    }

    if (!found) {
        searchSiteResultLabel->setText(tr("No encontrado"));
        searchSiteResultLabel->setStyleSheet("color : red");
        searchSiteResultLabel->setVisible(true);
    }
}

void FloorplanWidget::disableFocusOfElements() {
    addSelectedHWResourcesButton->setFocusPolicy(Qt::NoFocus);
    zoomInButton->setFocusPolicy(Qt::NoFocus);
    zoomOutButton->setFocusPolicy(Qt::NoFocus);
    acceptButton->setFocusPolicy(Qt::NoFocus);
    searchSiteButton->setFocusPolicy(Qt::NoFocus);
}
