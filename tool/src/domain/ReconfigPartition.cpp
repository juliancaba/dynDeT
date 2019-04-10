#include "ReconfigPartition.h"

using std::string;
using std::vector;

ReconfigPartition::ReconfigPartition() : partitionID("none"), name("none"), instanceName("none"), srcDir("none"), resetAfterReconfig(false), snappingMode(false) {}

ReconfigPartition::ReconfigPartition(string pID, string n, string instN, string srcD, bool reset, bool snapping) :
        partitionID(pID),
        name(n),
        instanceName(instN),
        srcDir(srcD),
        resetAfterReconfig(reset),
        snappingMode(snapping)
{}

ReconfigPartition::ReconfigPartition(const ReconfigPartition &p) :
        partitionID(p.getPartitionID()),
        name(p.getName()),
        instanceName(p.getInstanceName()),
        srcDir(p.getSrcDir()),
        resetAfterReconfig(p.getResetAfterReconfig()),
        snappingMode(p.getSnappingMode()),
        hwResources(p.getHwResources())
{}

ReconfigPartition& ReconfigPartition::operator=(const ReconfigPartition &p) {
    if (this != &p) {
        partitionID = p.getPartitionID();
        name = p.getName();
        instanceName = p.getInstanceName();
        srcDir = p.getSrcDir();
        resetAfterReconfig = p.getResetAfterReconfig();
        snappingMode = p.getSnappingMode();
        hwResources = p.getHwResources();
    }

    return *this;
}

void ReconfigPartition::addHwResource(string hwres) {
    hwResources.push_back(hwres);
}

string ReconfigPartition::getPartitionID() const {
    return partitionID;
}

string ReconfigPartition::getName() const {
    return name;
}

string ReconfigPartition::getInstanceName() const {
    return instanceName;
}

string ReconfigPartition::getSrcDir() const {
    return srcDir;
}

bool ReconfigPartition::getResetAfterReconfig() const {
    return resetAfterReconfig;
}

bool ReconfigPartition::getSnappingMode() const {
    return snappingMode;
}

vector<string> ReconfigPartition::getHwResources() const {
    return hwResources;
}

void ReconfigPartition::setPartitionID(string id) {
    partitionID = id;
}

void ReconfigPartition::setName(string n) {
    name = n;
}

void ReconfigPartition::setInstanceName(string instN) {
    instanceName = instN;
}

void ReconfigPartition::setSrcDir(string srcD) {
    srcDir = srcD;
}

void ReconfigPartition::setResetAfterReconfig(bool reset) {
    resetAfterReconfig = reset;
}

void ReconfigPartition::setSnappingMode(bool snapping) {
    snappingMode = snapping;
}
