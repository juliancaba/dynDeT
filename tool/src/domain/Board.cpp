#include "Board.h"
#include "BoardXML.h"

using std::string;
using std::vector;

Board::Board() : name("none"), rows(-1), cols(-1) {}

Board::Board(string n, int r, int c) : name(n), rows(r), cols(c) {}

Board::Board(const Board &b) :
        name(b.getName()),
        rows(b.getRows()),
        cols(b.getCols()),
        sites(b.getSites())
{}

bool Board::loadFromXML(string inputFile) {
    bool res;

    res = BoardXML::readBoard(inputFile, *this);

    return res;
}

void Board::addSite(Site &s) {
    sites.push_back(s);
}

std::string Board::getName() const {
    return name;
}

int Board::getRows() const {
    return rows;
}

int Board::getCols() const {
    return cols;
}

int Board::getNumOfSites() const {
    return sites.size();
}

Site Board::getSiteAt(int index) {
    return sites[index];
}

vector<Site> Board::getSites() const {
    return sites;
}

void Board::setName(string n) {
    name = n;
}

void Board::setRows(int r) {
    rows = r;
}

void Board::setCols(int c) {
    cols = c;
}
