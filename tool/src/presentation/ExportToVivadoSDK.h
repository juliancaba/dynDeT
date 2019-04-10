#ifndef EXPORTTOVIVADOSDK_H
#define EXPORTTOVIVADOSDK_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QCheckBox>
#include <QPushButton>
#include <QProgressBar>
#include <QProcess>
#include <QCloseEvent>

class ExportToVivadoSDK : public QDialog {
    Q_OBJECT
public:
    explicit ExportToVivadoSDK(QWidget *parent = 0, QString projectDir = QString());

private:
    QString mainDir;
    bool exportingNow;

    QGridLayout *layout;
    QLabel *titleLabel;
    QButtonGroup *bttnGroup;
    QCheckBox *addBitstreamOption;
    QCheckBox *launchSDKOption;
    QPushButton *exportButton;
    QProgressBar *exportProgressBar;

    QProcess *exportProcess;

    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void exportToSDK();
    void exportProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif
