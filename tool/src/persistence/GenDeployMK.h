#ifndef GENDEPLOYMK_H
#define GENDEPLOYMK_H

#include <vector>
#include <string>


class GenDeployMK {
public:
    static bool generate(std::string outputFile, std::string bitstreamsDir, std::vector<std::vector<std::string> > partialBitstreams);
};

#endif
