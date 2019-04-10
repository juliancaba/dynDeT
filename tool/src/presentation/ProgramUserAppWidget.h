#ifndef PROGRAMUSERAPPWIDGET_H
#define PROGRAMUSERAPPWIDGET_H

#include "ReconfManagementOptionInterface.h"
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QSpacerItem>
#include <QProgressBar>
#include <QProcess>

class ProgramUserAppWidget : public QWidget, public ReconfManagementOptionInterface {
    Q_OBJECT
public:
    explicit ProgramUserAppWidget(QWidget *parent = 0, QString projectDir = QString(), QString blockDesignFile = QString());
    bool getProgrammingNow();

private:
    QString mainDir;
    bool programmingNow;
    bool readPartialBitstreamAddrRangeOK;
    bool readBlockDesignFileOK;
    long partialBitstreamADDRMin;
    long partialBitstreamADDRMax;

    QGridLayout *layout;
    QGridLayout *hardwarePlatformLayout;
    QGridLayout *partialBitstreamsLayout;
    QGridLayout *appLayout;
    QGridLayout *fullBitstreamsLayout;
    QGroupBox *hardwarePlatformGroup;
    QGroupBox *partialBitstreamsGroup;
    QGroupBox *appGroup;
    QGroupBox *fullBitstreamsGroup;
    QPushButton *openSDKButton;
    QPushButton *addPartialBitstreamButton;
    QPushButton *programButton;
    QLabel *selectHWDefinitionLabel;
    QLabel *bitstreamName;
    QLabel *bitstreamMemAddress;
    QLabel *selectBitstreamToProgram;
    QLabel *selectAppLabel;
    QComboBox *selectHWDefinitionCBox;
    QComboBox *partialBitstreamsCBox;
    QComboBox *fullBitstreamsCBox;
    QLineEdit *bitstreamAddrLineEdit;
    QTableWidget *bitstreamsTable;
    QFileSystemModel *fileSystemModel;
    QTreeView *selectAppTreeView;
    QSpacerItem *verticalSpacer;
    QProgressBar *programProgressBar;

    QProcess *openSDKProcess;
    QProcess *programFullBitstrProcess;
    QProcess *buildFactoryBitstrsProcess;
    QProcess *deployProcess;

    void createHardwarePlatformGroup();
    void createPartialBitstreamsGroup();
    void readPartialBitstreamAddrRange(QString blockDesignFile);
    void createAppGroup();
    void createFullBitstreamsGroup();
    bool check();
    bool checkPartialBitstreamAddr();
    void setSelectableElementsEnabled(bool enabled);

    void startProgramFullBitstrProcess();
    void startBuildFactoryBitstrsProcess();
    void startDeployProcess();

private slots:
    void openSDK();
    void openSDKFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void addPartialBitstream();
    void showBitstreamsTableContextMenu(const QPoint &pos);
    void removeBitstreamsTableSelectedItems();
    void program();

    void programFullBitstrProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void buildFactoryBitstrsProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void deployProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif
