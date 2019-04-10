#ifndef LANGUAGESELECTOR_H
#define LANGUAGESELECTOR_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSpacerItem>

class LanguageSelector : public QDialog {
    Q_OBJECT
public:
    explicit LanguageSelector(QWidget *parent = 0);
    QString getSelectedLanguage();

private:
    QString selectedLanguage;

    QGridLayout *layout;
    QLabel *label;
    QComboBox *languages;
    QPushButton *acceptButton;
    QSpacerItem *spacer;

private slots:
    void newLanguageSelected(QString text);
};

#endif
