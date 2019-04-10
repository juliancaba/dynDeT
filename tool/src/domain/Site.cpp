#include "Site.h"

using std::string;

Site::Site() : name("none"), type("none"), clockRegion("none"), rpmX(-1), rpmY(-1) {}

Site::Site(string n, string t, string clkReg, int x, int y) :
        name(n),
        type(t),
        clockRegion(clkReg),
        rpmX(x),
        rpmY(y)
{}

Site::Site(const Site &s) :
        name(s.getName()),
        type(s.getType()),
        clockRegion(s.getClockRegion()),
        rpmX(s.getRPMX()),
        rpmY(s.getRPMY())
{}

string Site::getName() const {
    return name;
}

string Site::getType() const {
    return type;
}

string Site::getClockRegion() const {
    return clockRegion;
}

int Site::getRPMX() const {
    return rpmX;
}

int Site::getRPMY() const {
    return rpmY;
}

void Site::setName(string n) {
    name = n;
}

void Site::setType(string t) {
    type = t;
}

void Site::setClockRegion(string clkReg) {
    clockRegion = clkReg;
}

void Site::setRPMX(int x) {
    rpmX = x;
}

void Site::setRPMY(int y) {
    rpmY = y;
}

