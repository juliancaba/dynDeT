#ifndef PROGRAMFPGA_H
#define PROGRAMFPGA_H

#include <string>
#include <vector>

class ProgramFPGA {
private:
    std::string part;
    std::string bitstreamsDir;
    std::string fullBitstream;
    std::vector<std::vector<std::string> > partialBitstreams;
    std::string wrapperHWPlatform;
    std::vector<std::string> applications;

public:
    ProgramFPGA(std::string bitDir);
    ProgramFPGA(const ProgramFPGA &pfpga);

    bool generateProgramTCL(std::string outputFile);
    bool generateDeployTCL(std::string outputFile);
    bool generateSmallDeployTCL(std::string outputFile);
    bool generateDeployMK(std::string outputFile);
    bool generateSmallDeployMK(std::string outputFile);
    bool generateBootSDMK(std::string outputFile);

    void addPartialBitstream(std::string name, std::string memAddr);
    void addApplication(std::string app);
    void setPart(std::string p);
    void setFullBitstream(std::string bitstream);
    void setWrapperHWPlatform(std::string hwPlatform);

    std::string getPart() const;
    std::string getBitstreamsDir() const;
    std::string getFullBitstream() const;
    std::vector<std::vector<std::string> > getPartialBitstreams() const;
    std::string getWrapperHWPlatform() const;
    std::vector<std::string> getApplications() const;
};

#endif
