#include "GenDeployMK.h"
#include <fstream>
#include <ctemplate/template.h>

#define TEMPLATE_DEPLOY_FILE "/usr/share/dynDeT/static/template/deploymkTemplate.tpl"

using std::string;
using std::vector;
using std::ofstream;

bool GenDeployMK::generate(string outputFile, string bitstreamsDir, vector<vector<string> > partialBitstreams) {
    bool result = false;
    string output;
    ctemplate::TemplateDictionary dict("deploy.mk");
    ctemplate::TemplateDictionary *subdict;

    dict.SetValue("BITSTREAMS_DIR", bitstreamsDir);
    
#if defined VERSION==2015
    dict.AddSectionDictionary("VERSION_XMD");
#else
    dict.AddSectionDictionary("VERSION_XSDB");
#endif

    //Partial bitstreams
    for (size_t i = 0; i < partialBitstreams.size(); i++) {
        subdict = dict.AddSectionDictionary("PARTIAL_BITSTREAM");
        subdict->SetValue("PARTIAL_BITSTREAM_NAME", partialBitstreams[i][0]);
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
