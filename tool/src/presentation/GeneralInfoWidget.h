#ifndef GENERALINFOWIDGET_H
#define GENERALINFOWIDGET_H

#include "Design.h"
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

class GeneralInfoWidget : public QWidget {
    Q_OBJECT
public:
    explicit GeneralInfoWidget(QWidget *parent = 0, Design *des = 0);

    QString getPart();
    QString getBoard();
    QString getBlockDesign();
    void setProjectDir(QString &str);
    void enableAllElements();
    void disableFPGADataElements();
    void disableAllElements();
    void addToDesign();
    void addFPGADataToDesign();
    void addProjectDataToDesign();
    void loadFromDesign();
    bool check();

private:
    Design *mainDesign;
    QString mainDir;

    QGridLayout *layout;
    QGridLayout *fpgaDataGroupLayout;
    QGridLayout *projectDataGroupLayout;
    QGroupBox *fpgaDataGroup;
    QGroupBox *projectDataGroup;
    QLabel *partLabel;
    QLabel *boardLabel;
    QLabel *projectNameLabel;
    QLabel *designNameLabel;
    QLabel *ipDirLabel;
    QLabel *blockDesignLabel;
    QComboBox *partComboBox;
    QComboBox *boardComboBox;
    QLineEdit *projectNameLineEdit;
    QLineEdit *designNameLineEdit;
    QLineEdit *ipDirLineEdit;
    QLineEdit *blockDesignLineEdit;
    QPushButton *ipDirButton;
    QPushButton *blockDesignButton;


    bool checkLineEditEmpty(QLineEdit *le);
    void createFPGADataGroup();
    void createProjectDataGroup();

private slots:
    void listCompatibleBoards(const QString &part);
    void selectIPDir();
    void selectBlockDesignFile();

};

#endif
