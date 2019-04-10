#ifndef RECONFIGURATIONMANAGEMENT_H
#define RECONFIGURATIONMANAGEMENT_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>
#include <QSpacerItem>
#include <QPushButton>
#include <QCloseEvent>


#include <cstdlib>
using std::string;

class ReconfigurationManagement : public QDialog {
    Q_OBJECT
public:
    explicit ReconfigurationManagement(QWidget *parent = 0, QString projectDir = QString(), QString blockDesign = QString(), string bdname = NULL);

private:
    QString mainDir;
    QString blockDesignTCLFile;
    string bdname;

    QGridLayout *layout;
    QWidget *selectedOptionWidget;
    QLabel *titleLabel;
    QLabel *optionsLabel;
    QButtonGroup *bttnGroup;
    QRadioButton *programUsingThisToolButton;
    QRadioButton *programUserAppButton;
    QSpacerItem *spacer;
    QPushButton *nextButton;

    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void next();
};

#endif
