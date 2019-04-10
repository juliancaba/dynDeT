#include "Configuration.h"

using std::string;
using std::vector;

Configuration::Configuration() : configID("none"), name("none"), xilinxGeneralCompress(false) {}

Configuration::Configuration(string cID, string n) : configID(cID), name(n), xilinxGeneralCompress(false) {}

Configuration::Configuration(const Configuration &c) : configID(c.getConfigID()), name(c.getName()), modulesIDs(c.getModulesIDs()), xilinxGeneralCompress(c.getXilinxGeneralCompress()) {}

Configuration& Configuration::operator=(const Configuration &c) {
    if (this != &c) {
        configID = c.getConfigID();
        name = c.getName();
        modulesIDs = c.getModulesIDs();
        xilinxGeneralCompress = c.getXilinxGeneralCompress();
    }

    return *this;
}

void Configuration::addModule(std::string moduleID) {
    modulesIDs.push_back(moduleID);
}

string Configuration::getConfigID() const {
    return configID;
}

string Configuration::getName() const {
    return name;
}

vector<string> Configuration::getModulesIDs() const {
    return modulesIDs;
}

bool Configuration::getXilinxGeneralCompress() const {
    return xilinxGeneralCompress;
}

void Configuration::setConfigID(string id) {
    configID = id;
}

void Configuration::setName(string n) {
    name = n;
}

void Configuration::setXilinxGeneralCompress(bool value) {
    xilinxGeneralCompress = value;
}
