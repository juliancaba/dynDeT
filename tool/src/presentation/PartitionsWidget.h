#ifndef PARTITIONSWIDGET_H
#define PARTITIONSWIDGET_H

#include "Design.h"
#include "FloorplanWidget.h"
#include "ModulesWidget.h"
#include "ConfigurationsWidget.h"
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QSpacerItem>

//Forward declaration
class MainWindow;

class PartitionsWidget : public QWidget {
    Q_OBJECT
public:
    explicit PartitionsWidget(MainWindow *parent = 0, Design *des = 0);

    void enableAllEditElements();
    void disableAllEditElements();
    QStringList getPartitionsIDs();
    void loadFromDesign();
    void addAllPartitionsToDesign();
    bool check();
    void enablePartitionsContextMenu();
    void disablePartitionsContextMenu();
    void disableExistingPartitions();
    void enableExistingPartitions();

private:
    Design *mainDesign;
    MainWindow *mainWindowParent;
    FloorplanWidget fpWidget;

    QGridLayout *layout;
    QGridLayout *addPartitionGroupLayout;
    QGridLayout *partitionsGroupLayout;
    QGroupBox *addPartitionGroup;
    QGroupBox *partitionsGroup;
    QTableWidget *partitionsTable;
    QLabel *idLabel;
    QLabel *partiNameLabel;
    QLabel *partiInstanceLabel;
    QLabel *sourceDirLabel;
    QLabel *partitionPropertiesLabel;
    QLineEdit *idLineEdit;
    QLineEdit *partiNameLineEdit;
    QLineEdit *partiInstanceLineEdit;
    QLineEdit *sourceDirLineEdit;
    QButtonGroup *buttonGroup;
    QCheckBox *resetAfterReconfigCheckBox;
    QCheckBox *snappingModeCheckBox;
    QPushButton *addPartitionButton;
    QSpacerItem *spacer;

    //Hardware resources
    QLabel *hwResourcesLabel;
    QLineEdit *hwResourcesLineEdit;
    QPushButton *openFloorplanWidgetButton;

    bool checkLineEditEmpty(QLineEdit *le);
    bool checkPartitionIDOK();
    void createAddPartitionGroup();
    void createPartitionsGroup();
    void addPartitionToDesign();
    void addPartitionToTable();

private slots:
    void addPartition();
    void removePartition();
    void showPartitionsContextMenu(const QPoint &pos);
    void openFloorplanWidget();
    void setFlagsPartitions(QFlags<Qt::ItemFlag> flags);

signals:

public slots:

};

#endif
