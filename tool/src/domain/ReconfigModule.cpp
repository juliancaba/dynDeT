#include "ReconfigModule.h"

using std::string;
using std::vector;

ReconfigModule::ReconfigModule() : moduleID("none"), name("none"), partitionID("none") {}

ReconfigModule::ReconfigModule(string mID, string n, string partID) : moduleID(mID), name(n), partitionID(partID) {}

ReconfigModule::ReconfigModule(const ReconfigModule &m) :
        moduleID(m.getModuleID()),
        name(m.getName()),
        partitionID(m.getPartitionID()),
        files(m.getFiles())
{}

ReconfigModule& ReconfigModule::operator=(const ReconfigModule &m) {
    if (this != &m) {
        moduleID = m.getModuleID();
        name = m.getName();
        partitionID = m.getPartitionID();
        files = m.getFiles();
    }

    return *this;
}

void ReconfigModule::addFile(string dir, string fileName) {
    vector<string> aux;

    aux.push_back(dir);
    aux.push_back(fileName);

    files.push_back(aux);
}

string ReconfigModule::getModuleID() const {
    return moduleID;
}

string ReconfigModule::getName() const {
    return name;
}

string ReconfigModule::getPartitionID() const {
    return partitionID;
}

vector<vector<string> > ReconfigModule::getFiles() const {
    return files;
}

void ReconfigModule::setModuleID(string id) {
    moduleID = id;
}

void ReconfigModule::setName(string n) {
    name = n;
}

void ReconfigModule::setPartitionID(string partID) {
    partitionID = partID;
}
