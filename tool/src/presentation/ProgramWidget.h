#ifndef PROGRAMWIDGET_H
#define PROGRAMWIDGET_H

#include "ReconfManagementOptionInterface.h"
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QProgressBar>
#include <QProcess>


#include <cstdlib>
using std::string;

class ProgramWidget : public QWidget, public ReconfManagementOptionInterface {
    Q_OBJECT
public:
    explicit ProgramWidget(QWidget *parent = 0, QString projectDir = QString(), string blockDesignFile = NULL);
    bool getProgrammingNow();

private:
    QString mainDir;    
    string blockDesign;
    bool isFPGAProgrammed;
    bool programmingNow;

    QGridLayout *layout;
    QComboBox *bitstreamsCBox;
    QLabel *bitstreamsLabel;
    QPushButton *programButton;
    QProgressBar *programProgressBar;

    QProcess *programProcess;

    void createBitstreamCBox();
    void disableFocusOfElements();

private slots:
    void program();
    void programProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

};

#endif
