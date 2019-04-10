#ifndef SITE_H
#define SITE_H

#include <string>

class Site {
private:
    std::string name;
    std::string type;
    std::string clockRegion;
    int rpmX;
    int rpmY;

public:
    Site();
    Site(std::string n, std::string t, std::string clkReg, int x, int y);
    Site(const Site &s);

    std::string getName() const;
    std::string getType() const;
    std::string getClockRegion() const;
    int getRPMX() const;
    int getRPMY() const;

    void setName(std::string n);
    void setType(std::string t);
    void setClockRegion(std::string clkReg);
    void setRPMX(int x);
    void setRPMY(int y);
};

#endif
