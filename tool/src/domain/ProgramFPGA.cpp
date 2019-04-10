#include "ProgramFPGA.h"
#include "GenProgramTCL.h"
#include "GenDeployTCL.h"
#include "GenDeployMK.h"
#include "GenBootSDMK.h"

using std::string;
using std::vector;

ProgramFPGA::ProgramFPGA(string bitDir) : bitstreamsDir(bitDir) {}

ProgramFPGA::ProgramFPGA(const ProgramFPGA &pfpga) :
    part(pfpga.getPart()),
    bitstreamsDir(pfpga.getBitstreamsDir()),
    fullBitstream(pfpga.getFullBitstream()),
    partialBitstreams(pfpga.getPartialBitstreams()),
    wrapperHWPlatform(pfpga.getWrapperHWPlatform()),
    applications(pfpga.getApplications())
{}

bool ProgramFPGA::generateProgramTCL(string outputFile) {
    bool res;

    if (bitstreamsDir.empty() || fullBitstream.empty()) {
        res = false;
    }
    else {
        res = GenProgramTCL::generate(outputFile, bitstreamsDir, fullBitstream);
    }

    return res;
}

bool ProgramFPGA::generateDeployTCL(string outputFile) {
    bool res;
    vector<vector<string> > factoryBitstreams;

    if (partialBitstreams.empty() || wrapperHWPlatform.empty() || applications.empty()) {
        res = false;
    }
    else {
        //Create the factory bitstreams vector
        for (size_t i = 0; i < partialBitstreams.size(); i++) {
            vector<string> factoryBitstr;

            factoryBitstr.push_back("factory_" + partialBitstreams[i][0]);
            factoryBitstr.push_back(partialBitstreams[i][1]);

            factoryBitstreams.push_back(factoryBitstr);
        }

        res = GenDeployTCL::generate(outputFile, wrapperHWPlatform, bitstreamsDir, factoryBitstreams, applications);
    }

    return res;
}


bool ProgramFPGA::generateSmallDeployTCL(string outputFile) {
  //Create the factory bitstreams vector
  std::vector<std::string> emptyVector;
  std::vector<std::vector<std::string> > emptyVector2;
  
  return GenDeployTCL::generate(outputFile, wrapperHWPlatform, bitstreamsDir, emptyVector2, emptyVector);
}


bool ProgramFPGA::generateDeployMK(string outputFile) {
    bool res;

    if (partialBitstreams.empty()) {
        res = false;
    }
    else {
        res = GenDeployMK::generate(outputFile, bitstreamsDir, partialBitstreams);
    }

    return res;
}


bool ProgramFPGA::generateSmallDeployMK(string outputFile) {
    std::vector<std::vector<std::string> > emptyVector2;

    return GenDeployMK::generate(outputFile, bitstreamsDir, emptyVector2);
}

bool ProgramFPGA::generateBootSDMK(string outputFile) {
    bool res = false;

    if (applications.size() == 1) {
        res = GenBootSDMK::generate(outputFile, part, bitstreamsDir, fullBitstream, applications[0]);
    }

    return res;
}

void ProgramFPGA::addPartialBitstream(string name, string memAddr) {
    vector<string> aux;

    aux.push_back(name);
    aux.push_back(memAddr);

    partialBitstreams.push_back(aux);
}

void ProgramFPGA::addApplication(std::string app) {
    applications.push_back(app);
}

void ProgramFPGA::setPart(string p) {
    part = p;
}

void ProgramFPGA::setFullBitstream(string bitstream) {
    fullBitstream = bitstream;
}

void ProgramFPGA::setWrapperHWPlatform(string hwPlatform) {
    wrapperHWPlatform = hwPlatform;
}

string ProgramFPGA::getPart() const {
    return part;
}

string ProgramFPGA::getBitstreamsDir() const {
    return bitstreamsDir;
}

string ProgramFPGA::getFullBitstream() const {
    return fullBitstream;
}

vector<vector<string> > ProgramFPGA::getPartialBitstreams() const {
    return partialBitstreams;
}

string ProgramFPGA::getWrapperHWPlatform() const {
    return wrapperHWPlatform;
}

vector<string> ProgramFPGA::getApplications() const {
    return applications;
}
