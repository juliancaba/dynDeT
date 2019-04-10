#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GeneralInfoWidget.h"
#include "StaticPartWidget.h"
#include "PartitionsWidget.h"
#include "ModulesWidget.h"
#include "ConfigurationsWidget.h"
#include "BitstreamsWidget.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <QFrame>
#include <QProgressBar>
#include <QProcess>
#include <QMenuBar>
#include <QCloseEvent>
#include <QTreeWidget>
#include <QSpacerItem>


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    QString getBoard();
    QStringList getModulesByPartition(QString partitionID);
    QStringList getConfigurationsByPartition(QString partitionID);
    QStringList getConfigurationsByModule(QString moduleID);
    void removeModulesByPartition(QString partitionID);
    void removeConfigurationsByPartition(QString partitionID);
    void removeConfigurationsByModule(QString moduleID);

private:
    Design mainDesign;
    bool partialDesign;
    bool cleanedPrj;
    int currentFlowIndex;
    int currentFlowTreeWidgetIndex;

    QMenuBar *menubar;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QAction *createBootImageSDAct;
    QAction *exportToVivadoSDKAct;
    QAction *openReconfManagementAct;
    QAction *aboutAct;
    QWidget *mainWidget;
    QTreeWidget *flowTreeWidget;
    QStackedWidget *stackedWidget;
    GeneralInfoWidget *generalInfoWidget;
    StaticPartWidget *staticPartWidget;
    PartitionsWidget *partitionsWidget;
    ModulesWidget *modulesWidget;
    ConfigurationsWidget *configurationsWidget;
    BitstreamsWidget *bitstreamsWidget;
    QLabel *titleLabel;
    QLabel *projectDirLabel;
    QLabel *makeProjectLabel;
    QLineEdit *projectDirLineEdit;
    QGridLayout *layout;
    QPushButton *projectDirButton;
    QPushButton *projectDirAcceptButton;
    QPushButton *continueButton;
    QPushButton *generateButton;
    QPushButton *cleanButton;
    QFrame *hLineTop;
    QFrame *hLineBottom;
    QProgressBar *makeProjectProgressBar;
    QSpacerItem *spacer;

    QProcess *makeProcess;

    void createTitle();
    void createSelectProjectDirectory();
    void createFlowTreeWidget();
    void createStackedWidget();
    void createButtons();
    void createLines();
    void createProgressBar();
    void createActions();
    void createMenus();

    void enableMenus();
    void disableMenus();

    bool checkFlowIndex(int index);
    void setCurrentFlowTreeWidgetItem(int index);
    void evalContinueButton();

    void enableFlowAfterAnError();
    void enablePartialFlowAfterAnError();

    void closeEvent(QCloseEvent *event);

private slots:
    void selectProjectDir();
    void acceptProjectDir();
    void flowTreeWidgetItemClicked(QTreeWidgetItem *item, int column);
    void cont();
    void generate();
    void cleanDesign();
    void makeProjectFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void openCreateBootImageSD();
    void openExportToVivadoSDK();
    void openReconfManagement();
    void openAbout();
};

#endif
