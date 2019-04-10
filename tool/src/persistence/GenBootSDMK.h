#ifndef GENBOOTSDMK_H
#define GENBOOTSDMK_H

#include <string>

class GenBootSDMK {
public:
    static bool generate(std::string outputFile, std::string part, std::string bitstreamsDir, std::string fullBitstream, std::string userApp);
};

#endif
