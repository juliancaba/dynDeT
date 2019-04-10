#include "LanguageSelector.h"

#define LANGUAGE_SPANISH_LARGE "Español"
#define LANGUAGE_ENGLISH_LARGE "English"

#define LANGUAGE_SPANISH "es"
#define LANGUAGE_ENGLISH "en"

LanguageSelector::LanguageSelector(QWidget *parent) : QDialog(parent), selectedLanguage("none") {
    label = new QLabel("Select a language:", this);

    //Combobox
    languages = new QComboBox(this);
    languages->addItem(LANGUAGE_SPANISH_LARGE);
    languages->addItem(LANGUAGE_ENGLISH_LARGE);
    connect(languages, SIGNAL(currentTextChanged(QString)), this, SLOT(newLanguageSelected(QString)));

    //Accept button
    acceptButton = new QPushButton("OK", this);
    acceptButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));

    //Spacer
    spacer = new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Set the layout
    layout = new QGridLayout();
    layout->addWidget(label, 0, 0, 1, 2);
    layout->addWidget(languages, 1, 0, 1, 2);
    layout->addItem(spacer, 2, 0);
    layout->addWidget(acceptButton, 3, 1, 1, 1, Qt::AlignRight);
    setLayout(layout);

    //Window
    setWindowTitle("Language selector - Development tool of dynamically reconfigurable applications in FPGAs");
    resize(400, 150);
}

void LanguageSelector::newLanguageSelected(QString text) {
    if (text == LANGUAGE_SPANISH_LARGE) {
        selectedLanguage = LANGUAGE_SPANISH;
    }

    if (text == LANGUAGE_ENGLISH_LARGE) {
        selectedLanguage = LANGUAGE_ENGLISH;
    }
}

QString LanguageSelector::getSelectedLanguage() {
    return selectedLanguage;
}
