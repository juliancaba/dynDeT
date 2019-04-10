#ifndef CREATEBOOTIMAGESD_H
#define CREATEBOOTIMAGESD_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QTreeView>
#include <QFileSystemModel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QCloseEvent>
#include <QSpacerItem>
#include <QProgressBar>

class CreateBootImageSD : public QDialog {
    Q_OBJECT
public:
    explicit CreateBootImageSD(QWidget *parent = 0, QString projectDir = QString(), QString part = QString());

private:
    QString mainDir;
    QString boardPart;
    bool creatingBootImageNow;

    QGridLayout *layout;
    QGridLayout *appLayout;
    QGridLayout *fullBitstreamsLayout;
    QGridLayout *outputDirLayout;
    QGroupBox *appGroup;
    QGroupBox *fullBitstreamsGroup;
    QGroupBox *outputDirGroup;
    QLabel *titleLabel;
    QLabel *selectAppLabel;
    QLabel *selectBitstream;
    QLabel *outputDirLabel;
    QFileSystemModel *fileSystemModel;
    QTreeView *selectAppTreeView;
    QComboBox *fullBitstreamsCBox;
    QLineEdit *outputDirLineEdit;
    QPushButton *openSDKButton;
    QPushButton *createBootImageButton;
    QPushButton *outputDirButton;
    QSpacerItem *spacer;
    QProgressBar *createBootImageProgressBar;

    QProcess *openSDKProcess;
    QProcess *createBootImageProcess;

    void createAppGroup();
    void createFullBitstreamsGroup();
    void createOutputDirGroup();

    bool check();
    void setSelectableElementsEnabled(bool enabled);

    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void selectOutputDir();
    void openSDK();
    void openSDKFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void createBootImage();
    void createBootImageFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif
