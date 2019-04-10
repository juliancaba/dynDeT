#include "GenProgramTCL.h"
#include <fstream>
#include <ctemplate/template.h>

#define TEMPLATE_PROGRAM_FILE "/usr/share/dynDeT/static/template/programTemplate.tpl"

using std::string;
using std::ofstream;

bool GenProgramTCL::generate(string outputFile, string bitDir, string bitName) {
    bool result = false;
    string output;
    ctemplate::TemplateDictionary dict("program.tcl");

    //Bitstream data
    dict.SetValue("BIT_DIR", bitDir);
    dict.SetValue("BIT_NAME", bitName);

    //Generate
    ctemplate::ExpandTemplate(TEMPLATE_PROGRAM_FILE, ctemplate::DO_NOT_STRIP, &dict, &output);

    if (!output.empty()) {
        ofstream file(outputFile.c_str());
        file << output;
        file.close();

        result = true;
    }

    return result;
}
