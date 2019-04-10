#ifndef CONFIGURATIONSWIDGET_H
#define CONFIGURATIONSWIDGET_H

#include "Design.h"
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QSpacerItem>
#include <QCheckBox>

class ConfigurationsWidget : public QWidget {
    Q_OBJECT
public:
    explicit ConfigurationsWidget(QWidget *parent = 0, Design *des = 0);

    void addModulesData(QStringList &partitionsIDs, QList<QStringList> &modData);
    void addBlankingOptionToDesign();
    bool getBlankingOption();
    void enableBlankingOption();
    void disableBlankingOption();
    void enableConfigurationElements();
    void disableAllEditElements();
    void loadFromDesign();
    bool addAllConfigurationsToDesign();
    bool check();
    QStringList getNewBitstreams();
    void clearNewBitstreamsList();
    void enableConfigurationsContextMenu();
    void disableConfigurationsContextMenu();
    void disableExistingConfigurations();
    void enableExistingConfigurations();

    QStringList getConfigurationsByPartition(QString partitionID);
    QStringList getConfigurationsByModule(QString moduleID);
    bool removeConfigurationsByPartition(QString partitionID);
    bool removeConfigurationsByModule(QString moduleID);

private:
    Design *mainDesign;
    QStringList newBitstreams;

    QGridLayout *layout;
    QGridLayout *addConfigLayout;
    QGridLayout *configsGroupLayout;
    QGridLayout *otherOptionsGroupLayout;
    QGroupBox *addConfigGroup;
    QGroupBox *configsGroup;
    QGroupBox *otherOptionsGroup;
    QTableWidget *modulesToImplementTable;
    QTableWidget *configsTable;
    QLabel *configIDLabel;
    QLabel *configNameLabel;
    QLabel *modulesToImplementLabel;
    QLineEdit *configIDLineEdit;
    QLineEdit *configNameLineEdit;
    QPushButton *addConfigButton;
    QCheckBox *generateBlankingConfigurationCheckBox;
    QSpacerItem *addConfigGroupSpacer;

    void createAddConfigGroup();
    void createConfigsGroup();
    void createOtherOptionsGroup();
    void addConfigToDesign();
    void addConfigToTable();
    void setFlagsConfigurations(QFlags<Qt::ItemFlag> flags);

private slots:
    void addConfig();
    void removeConfig();
    void showConfigContextMenu(const QPoint &pos);

signals:

public slots:

};

#endif
