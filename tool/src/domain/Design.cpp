#include "Design.h"
#include "GenDesignTCL.h"
#include "DesignXML.h"
#include "config.h"

#include <cstdlib>

using std::string;
using std::vector;

Design::Design() :
        part("none"),
        board("none"),
        projectName("none"),
        designName("none"),
        ipDir("none"),
        blockDesignTCL("none"),
        createBlanking(false),
        referenceXilinxCompression(false),
        blankingXilinxCompression(false)
{}

Design::Design(const Design &d) :
        part(d.getPart()),
        board(d.getBoard()),
        projectName(d.getProjectName()),
        designName(d.getDesignName()),
        ipDir(d.getIPDir()),
        blockDesignTCL(d.getBlockDesignTCL()),
        staticFiles(d.getStaticFiles()),
        constraintFiles(d.getConstraintFiles()),
        partitions(d.getPartitions()),
        modules(d.getModules()),
        configurations(d.getConfigurations()),
        createBlanking(d.getCreateBlanking()),
        referenceXilinxCompression(d.getReferenceXilinxCompression()),
        blankingXilinxCompression(d.getBlankingXilinxCompression())
{}

Design& Design::operator=(const Design &d) {
    if (this != &d) {
        part = d.getPart();
        board = d.getBoard();
        projectName = d.getProjectName();
        designName = d.getDesignName();
        ipDir = d.getIPDir();
        blockDesignTCL = d.getBlockDesignTCL();
        staticFiles = d.getStaticFiles();
        constraintFiles = d.getConstraintFiles();
        partitions = d.getPartitions();
        modules = d.getModules();
        configurations = d.getConfigurations();
        createBlanking = d.getCreateBlanking();
        referenceXilinxCompression = d.getReferenceXilinxCompression();
        blankingXilinxCompression = d.getBlankingXilinxCompression();
    }

    return *this;
}

bool Design::copyFile(std::string outputFile) {
    bool res = true;

    std::string cmd = "cp -r ";
    cmd.append(INSTALL_DIR_STATIC_FILES);
    cmd.append("/* ");
    cmd.append(outputFile);
    
    if(std::system(cmd.c_str()) == EXIT_FAILURE)
      res=false;
    
    return res;
}

bool Design::generateTCL(string outputFile) {
    bool res;

    res = GenDesignTCL::generate(outputFile, *this);

    return res;
}

bool Design::generateXML(string outputFile) {
    bool res;

    res = DesignXML::saveDesign(outputFile, *this);

    return res;
}

bool Design::loadFromXML(string inputFile) {
    bool res;

    res = DesignXML::readDesign(inputFile, *this);

    return res;
}

void Design::setPart(string p) {
    part = p;
}

void Design::setBoard(string b) {
    board = b;
}

void Design::setProjectName(string prjname) {
    projectName = prjname;
}

void Design::setDesignName(string dname) {
    designName = dname;
}

void Design::setIPDir(string dir) {
    ipDir = dir;
}

void Design::setBlockDesignTCL(string blockdes) {
    blockDesignTCL = blockdes;
}

void Design::addStaticFile(string dir, string name) {
    vector<string> aux;

    aux.push_back(dir);
    aux.push_back(name);

    staticFiles.push_back(aux);
}

void Design::addConstraintFile(string dir, string name) {
    vector<string> aux;

    aux.push_back(dir);
    aux.push_back(name);

    constraintFiles.push_back(aux);
}

void Design::addPartition(ReconfigPartition &partition) {
    partitions.push_back(partition);
}

bool Design::addModule(ReconfigModule &module) {
    bool partitionOK = false;

    for (size_t i = 0; i < partitions.size() && !partitionOK; i++) {
        if (module.getPartitionID() == partitions[i].getPartitionID()) {
            partitionOK = true;
            modules.push_back(module);
        }
    }

    return partitionOK;
}

bool Design::addConfiguration(Configuration &config) {
    vector<string> modulesIDs = config.getModulesIDs();
    vector<bool> moduleOK(modulesIDs.size(), false);
    bool allModulesOK;

    for (size_t i = 0; i < modulesIDs.size(); i++) {
        for(size_t j = 0; j < modules.size() && !moduleOK[i]; j++) {
            if (modulesIDs[i] == modules[j].getModuleID()) {
                moduleOK[i] = true;
            }
        }
    }

    allModulesOK = true;
    for (size_t i = 0; i < moduleOK.size() && allModulesOK; i++) {
        allModulesOK = allModulesOK && moduleOK[i];
    }

    if (allModulesOK) {
        configurations.push_back(config);
    }

    return allModulesOK;
}

void Design::setCreateBlanking(bool val) {
    createBlanking = val;
}

void Design::setReferenceXilinxCompression(bool val) {
    referenceXilinxCompression = val;
}

void Design::setBlankingXilinxCompression(bool val) {
    blankingXilinxCompression = val;
}

bool Design::setXilinxCompression(string configID, bool value) {
    bool result = false;

    for (size_t i = 0; i < configurations.size() && !result; i++) {
        if (configID == configurations[i].getConfigID()) {
            configurations[i].setXilinxGeneralCompress(value);
            result = true;
        }
    }

    return result;
}

string Design::getPart() const {
    return part;
}

string Design::getBoard() const {
    return board;
}

string Design::getProjectName() const {
    return projectName;
}

string Design::getDesignName() const {
    return designName;
}

string Design::getIPDir() const {
    return ipDir;
}

string Design::getBlockDesignTCL() const {
    return blockDesignTCL;
}

vector<vector<string> > Design::getStaticFiles() const {
    return staticFiles;
}

vector<vector<string> > Design::getConstraintFiles() const {
    return constraintFiles;
}

ReconfigPartition Design::getPartition(string id) const {
    bool found = false;
    ReconfigPartition result;

    for (size_t i = 0; i < partitions.size() && !found; i++) {
        if (id == partitions[i].getPartitionID()) {
            result = partitions[i];
            found = true;
        }
    }

    return result;
}

ReconfigModule Design::getModule(string id) const {
    bool found = false;
    ReconfigModule result;

    for (size_t i = 0; i < modules.size() && !found; i++) {
        if (id == modules[i].getModuleID()) {
            result = modules[i];
            found = true;
        }
    }

    return result;
}

Configuration Design::getConfiguration(string id) const {
    bool found = false;
    Configuration result;

    for (size_t i = 0; i < configurations.size() && !found; i++) {
        if (id == configurations[i].getConfigID()) {
            result = configurations[i];
            found = true;
        }
    }

    return result;
}

vector<ReconfigPartition> Design::getPartitions() const {
    return partitions;
}

vector<ReconfigModule> Design::getModules() const {
    return modules;
}

vector<Configuration> Design::getConfigurations() const {
    return configurations;
}

bool Design::getCreateBlanking() const {
    return createBlanking;
}

bool Design::getReferenceXilinxCompression() const {
    return referenceXilinxCompression;
}

bool Design::getBlankingXilinxCompression() const {
    return blankingXilinxCompression;
}
