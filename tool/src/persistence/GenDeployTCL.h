#ifndef GENDEPLOYTCL_H
#define GENDEPLOYTCL_H

#include <vector>
#include <string>


class GenDeployTCL {
public:
    static bool generate(std::string outputFile, std::string wrapperHWSource, std::string bitstreamsDir, std::vector<std::vector<std::string> > factoryBitstreams, std::vector<std::string> apps);
};

#endif
