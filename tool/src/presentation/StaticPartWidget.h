#ifndef STATICPARTWIDGET_H
#define STATICPARTWIDGET_H

#include "Design.h"
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QListWidget>
#include <QPushButton>
#include <QButtonGroup>

class StaticPartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StaticPartWidget(QWidget *parent = 0, Design *des = 0);

    void setProjectDir(QString &str);
    void enableAllEditElements();
    void disableAllEditElements();
    void addToDesign();
    void loadFromDesign();
    bool check();

private:
    Design *mainDesign;
    QString mainDir;

    QGridLayout *layout;
    QGridLayout *staticFilesGroupLayout;
    QGridLayout *constraintFilesGroupLayout;
    QGroupBox *staticFilesGroup;
    QGroupBox *constraintFilesGroup;
    QTableWidget *staticFilesTable;
    QButtonGroup *topCheckBoxesGroup;
    QPushButton *addStaticFileButton;
    QPushButton *removeStaticFileButton;
    QListWidget *constraintFilesList;
    QPushButton *addConstraintFileButton;
    QPushButton *removeConstraintFileButton;

    void createStaticFilesGroup();
    void createConstraintFilesGroup();

    QString getPathOfFile(QString &file);
    QString getNameOfFile(QString &file);

private slots:
    void addStaticFiles();
    void removeStaticFile();
    void addConstraintFiles();
    void removeConstraintFile();
};

#endif
