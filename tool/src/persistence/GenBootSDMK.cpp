#include "GenBootSDMK.h"
#include <fstream>
#include <ctemplate/template.h>

#define TEMPLATE_BOOTSD_FILE "/usr/share/dynDeT/static/template/bootSDmkTemplate.tpl"

using std::string;
using std::ofstream;

bool GenBootSDMK::generate(string outputFile, string part, string bitstreamsDir, string fullBitstream, string userApp) {
    bool result = false;
    string output;
    ctemplate::TemplateDictionary dict("bootSD.mk");

    dict.SetValue("PART", part);
    dict.SetValue("FULL_BITSTREAM", "../" + bitstreamsDir + fullBitstream);
    dict.SetValue("USER_APP", "../" + userApp);

    //Generate
    ctemplate::ExpandTemplate(TEMPLATE_BOOTSD_FILE, ctemplate::DO_NOT_STRIP, &dict, &output);

    if (!output.empty()) {
        ofstream file(outputFile.c_str());
        file << output;
        file.close();

        result = true;
    }

    return result;
}
