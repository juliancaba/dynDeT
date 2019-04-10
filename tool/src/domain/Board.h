#ifndef BOARD_H
#define BOARD_H

#include "Site.h"
#include <string>
#include <vector>

class Board {
private:
    std::string name;
    int rows;
    int cols;
    std::vector<Site> sites;

public:
    Board();
    Board(std::string n, int r, int c);
    Board(const Board &b);

    bool loadFromXML(std::string inputFile);

    void addSite(Site &s);

    std::string getName() const;
    int getRows() const;
    int getCols() const;
    int getNumOfSites() const;
    Site getSiteAt(int index);
    std::vector<Site> getSites() const;

    void setName(std::string n);
    void setRows(int r);
    void setCols(int c);
};

#endif
