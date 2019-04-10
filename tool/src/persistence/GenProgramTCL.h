#ifndef GENPROGRAMTCL_H
#define GENPROGRAMTCL_H

#include <string>

class GenProgramTCL {
public:
    static bool generate(std::string outputFile, std::string bitDir, std::string bitName);
};

#endif
