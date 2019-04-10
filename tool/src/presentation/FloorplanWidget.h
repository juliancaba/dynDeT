#ifndef FLOORPLANWIDGET_H
#define FLOORPLANWIDGET_H

#include "Board.h"
#include "SelectionDelegate.h"
#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QTableWidget>
#include <QTreeWidget>
#include <QBrush>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>

class FloorplanWidget : public QDialog {
    Q_OBJECT
public:
    explicit FloorplanWidget(QWidget *parent = 0);

    bool loadBoardData(QString board);
    QString getHWResources();
    void disableAddedHWResources();
    bool getBoardIsLoaded();
    void clearHWResData();
    void removePartitionArea(int index);

private:
    Board board;
    QList<QTableWidgetItem *> lastAddedHWResources;
    QList<QList<QTableWidgetItem *> > occupiedHWResources;
    bool boardIsLoaded;

    QGridLayout *layout;
    QGridLayout *boardGroupLayout;
    QGridLayout *toolsGroupLayout;
    QGridLayout *searchSiteGroupLayout;
    QGridLayout *hwResourcesGroupLayout;
    QGroupBox *boardGroup;
    QGroupBox *toolsGroup;
    QGroupBox *searchSiteGroup;
    QGroupBox *hwResourcesGroup;
    QTableWidget *boardTable;
    QPushButton *addSelectedHWResourcesButton;
    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;
    QLineEdit *searchSiteLineEdit;
    QPushButton *searchSiteButton;
    QLabel *searchSiteResultLabel;
    QTreeWidget *hwResourcesTree;
    QLabel *titleLabel;
    QFrame *hLineBottom;
    QPushButton *acceptButton;

    void createBoardGroup();
    void createToolsGroup();
    void createSearchSiteGroup();
    void createHWResourcesGroup();

    void selectSiteColor(QBrush &brush, QString type);
    void fillEmptyCells();

    void disableFocusOfElements();

private slots:
    void addSelectedHWResourcesToTreeWidget();
    void zoomIn();
    void zoomOut();
    void showHWResourcesTreeWidgetContextMenu(const QPoint &pos);
    void removeHWResourcesTreeWidgetSelectedItems();
    void searchSite();

signals:

public slots:

};

#endif
