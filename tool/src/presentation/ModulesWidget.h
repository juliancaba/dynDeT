#ifndef MODULESWIDGET_H
#define MODULESWIDGET_H

#include "Design.h"
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include <QSpacerItem>

//Forward declaration
class MainWindow;

class ModulesWidget : public QWidget {
    Q_OBJECT
public:
    explicit ModulesWidget(MainWindow *parent = 0, Design *des = 0);

    void setProjectDir(QString &str);
    void addPartitionsToComboBox(QStringList &qstrList);
    void enableAllEditElements();
    void disableAllEditElements();
    QList<QStringList> getModulesData();
    void loadFromDesign();
    bool addAllModulesToDesign();
    bool check();
    void enableModulesContextMenu();
    void disableModulesContextMenu();
    void disableExistingModules();
    void enableExistingModules();

    QStringList getModulesByPartition(QString partitionID);
    bool removeModulesByPartition(QString partitionID);

private:
    Design *mainDesign;
    MainWindow *mainWindowParent;
    QString mainDir;

    QGridLayout *layout;
    QGridLayout *addModuleLayout;
    QGridLayout *modulesGroupLayout;
    QGroupBox *addModuleGroup;
    QGroupBox *modulesGroup;
    QTableWidget *modulesTable;
    QLabel *modIDLabel;
    QLabel *modNameLabel;
    QLabel *modPartitionLabel;
    QLabel *modFilesLabel;
    QLineEdit *modIDLineEdit;
    QLineEdit *modNameLineEdit;
    QComboBox *modPartitionComboBox;
    QListWidget *modFilesList;
    QPushButton *addFileButton;
    QPushButton *addModuleButton;
    QSpacerItem *addModuleGroupSpacer;

    void createAddModuleGroup();
    void createModulesGroup();
    void addModuleToDesign();
    void addModuleToTable();

    QString getPathOfFile(QString &file);
    QString getNameOfFile(QString &file);

private slots:
    void addModule();
    void removeModule();
    void showModulesContextMenu(const QPoint &pos);
    void addModuleFiles();
    void showFileListContextMenu(const QPoint &pos);
    void removeFileListSelectedItems();
    void setFlagsModules(QFlags<Qt::ItemFlag> flags);

signals:

public slots:

};

#endif
