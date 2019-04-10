#include "GenDeployTCL.h"
#include <fstream>
#include <ctemplate/template.h>

#if defined VERSION==2015
#define TEMPLATE_DEPLOY_FILE "/usr/share/dynDeT/static/template/xmdTemplate.tpl"
#else
#define TEMPLATE_DEPLOY_FILE "/usr/share/dynDeT/static/template/xsdbTemplate.tpl"
#endif

using std::string;
using std::vector;
using std::ofstream;


bool GenDeployTCL::generate(string outputFile, string wrapperHWSource, string bitstreamsDir, vector<vector<string> > factoryBitstreams, vector<string> apps) {
    bool result = false;
    string output;
#if defined VERSION==2015
    ctemplate::TemplateDictionary dict("xmd.tcl");
#else
    ctemplate::TemplateDictionary dict("xsdb.tcl");
#endif
    ctemplate::TemplateDictionary *factoryBitstrSubdict, *appsSubdict;

    //Bitstream data
    dict.SetValue("WRAPPER_HW_PLATFORM_SOURCE", wrapperHWSource);

    //Factory bitstreams
    for (size_t i = 0; i < factoryBitstreams.size(); i++) {
        factoryBitstrSubdict = dict.AddSectionDictionary("DOW_FACTORY_BITSTREAM");
        factoryBitstrSubdict->SetValue("BITSTREAMS_DIR", bitstreamsDir);
        factoryBitstrSubdict->SetValue("FACTORY_BITSTREAM", factoryBitstreams[i][0]);
        factoryBitstrSubdict->SetValue("MEM_ADDR", factoryBitstreams[i][1]);
    }

    //Apps
    for (size_t i = 0; i < apps.size(); i++) {
        appsSubdict = dict.AddSectionDictionary("DOW_APP");
        appsSubdict->SetValue("APP", apps[i]);
    }

    //Generate
    ctemplate::ExpandTemplate(TEMPLATE_DEPLOY_FILE, ctemplate::DO_NOT_STRIP, &dict, &output);

    if (!output.empty()) {
        ofstream file(outputFile.c_str());
        file << output;
        file.close();

        result = true;
    }

    return result;
}
