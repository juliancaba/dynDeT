#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include "LanguageSelector.h"
#include "MainWindow.h"

#define LANGUAGES_DIR "/usr/share/dynDeT/static/languages/"

int main(int argc, char *argv[]) {
    int returnLangSelector;
    QString language;

    QApplication app(argc, argv);
    QTranslator translator;

    LanguageSelector langSelector;
    returnLangSelector = langSelector.exec();

    if (returnLangSelector == QDialog::Accepted) {
        language = langSelector.getSelectedLanguage();

        //If a translation file can be loaded, translate the app. If not, use the default language (spanish).
        if (translator.load("dyndetLangQt_" + language, LANGUAGES_DIR)) {
            app.installTranslator(&translator);
        }
    }


    MainWindow window;
    window.show();

    return app.exec();
}
