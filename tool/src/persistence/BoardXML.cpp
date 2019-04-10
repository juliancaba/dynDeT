#include "BoardXML.h"
#include <QFile>
#include <QString>
#include <QXmlStreamReader>

#define BOARD_XML_ELEMENT "Board"
#define BOARD_NAME_XML_ATTRIBUTE "name"
#define BOARD_ROWS_XML_ATTRIBUTE "rows"
#define BOARD_COLUMNS_XML_ATTRIBUTE "columns"
#define SITE_XML_ELEMENT "Site"
#define SITE_NAME_XML_ATTRIBUTE "name"
#define SITE_TYPE_XML_ATTRIBUTE "type"
#define SITE_CLK_REGION_XML_ATTRIBUTE "clkreg"
#define SITE_RPM_X_XML_ATTRIBUTE "rpmx"
#define SITE_RPM_Y_XML_ATTRIBUTE "rpmy"

using std::string;


bool BoardXML::readBoard(string inputFile, Board &board) {
    bool result = false;
    QFile file(QString::fromStdString(inputFile));

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QXmlStreamReader xmlReader(&file);
        QXmlStreamAttributes xmlAttrs;

        while (!xmlReader.atEnd() && !xmlReader.hasError()) {
            if (xmlReader.isStartElement()) {
                //Board element
                if (xmlReader.name() == BOARD_XML_ELEMENT) {
                    xmlAttrs = xmlReader.attributes();
                    readBoardAttrs(xmlAttrs, board);
                }

                //Site element
                if (xmlReader.name() == SITE_XML_ELEMENT) {
                    xmlAttrs = xmlReader.attributes();
                    readSiteAttrs(xmlAttrs, board);
                }
            }

            xmlReader.readNext();
        }

        if (!xmlReader.hasError()) {
            result = true;
        }

        file.close();
        xmlReader.clear();
    }

    return result;
}

void BoardXML::readBoardAttrs(QXmlStreamAttributes &boardAttrs, Board &board) {
    QString name;
    int rows, cols;

    name = boardAttrs.value(BOARD_NAME_XML_ATTRIBUTE).toString();
    rows = boardAttrs.value(BOARD_ROWS_XML_ATTRIBUTE).toInt();
    cols = boardAttrs.value(BOARD_COLUMNS_XML_ATTRIBUTE).toInt();

    board.setName(name.toStdString());
    board.setRows(rows);
    board.setCols(cols);
}

void BoardXML::readSiteAttrs(QXmlStreamAttributes &siteAttrs, Board &board) {
    QString name, type, clkReg;
    int rpmX, rpmY;

    name = siteAttrs.value(SITE_NAME_XML_ATTRIBUTE).toString();
    type = siteAttrs.value(SITE_TYPE_XML_ATTRIBUTE).toString();
    clkReg = siteAttrs.value(SITE_CLK_REGION_XML_ATTRIBUTE).toString();
    rpmX = siteAttrs.value(SITE_RPM_X_XML_ATTRIBUTE).toInt();
    rpmY = siteAttrs.value(SITE_RPM_Y_XML_ATTRIBUTE).toInt();

    Site site(name.toStdString(), type.toStdString(), clkReg.toStdString(), rpmX, rpmY);

    board.addSite(site);
}
