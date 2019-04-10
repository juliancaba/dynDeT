#ifndef BOARDXML_H
#define BOARDXML_H

#include "Board.h"
#include <string>
#include <QXmlStreamAttributes>

class BoardXML {
private:
    static void readBoardAttrs(QXmlStreamAttributes &boardAttrs, Board &board);
    static void readSiteAttrs(QXmlStreamAttributes &siteAttrs, Board &board);

public:
    static bool readBoard(std::string inputFile, Board &board);
};

#endif
