#ifndef SRC_GENDESIGNTCL_H_
#define SRC_GENDESIGNTCL_H_

#include "Design.h"
#include <ctemplate/template.h>


class GenDesignTCL {
private:
    static void addStaticFilesToDict(ctemplate::TemplateDictionary &dict, Design &des);
    static void addConstraintFilesToDict(ctemplate::TemplateDictionary &dict, Design &des);
    static void addPartitionsToDict(ctemplate::TemplateDictionary &dict, Design &des);
    static void addModulesToDict(ctemplate::TemplateDictionary &dict, Design &des);
    static void addConfigurationsToDict(ctemplate::TemplateDictionary &dict, Design &des);

public:
    static bool generate(std::string outputFile, Design &des);
};


#endif
