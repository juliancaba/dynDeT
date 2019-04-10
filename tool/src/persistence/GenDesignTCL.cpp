#include "GenDesignTCL.h"
#include <fstream>

#define TEMPLATE_DESIGN_FILE "/usr/share/dynDeT/static/template/designTemplate.tpl"

using std::vector;
using std::string;
using std::ofstream;


bool GenDesignTCL::generate(string outputFile, Design &des) {
    bool result = false;
    string output;
    ctemplate::TemplateDictionary dict("design.tcl");

    //Board data
    dict.SetValue("PART", des.getPart());
    dict.SetValue("BOARD", des.getBoard());

    //Vivado project data
    dict.SetValue("PROJECT_NAME", "tmp");//des.getProjectName());
    dict.SetValue("DESIGN_NAME", des.getDesignName());
    dict.SetValue("USER_IP_DIR", des.getIPDir());
    dict.SetValue("BLOCK_DESIGN_TCL", des.getBlockDesignTCL());

    //Static files
    addStaticFilesToDict(dict, des);

    //Constraint files
    addConstraintFilesToDict(dict, des);

    //Partitions
    addPartitionsToDict(dict, des);

    //Modules
    addModulesToDict(dict, des);

    //Configurations
    addConfigurationsToDict(dict, des);

    //Blanking configuration
    dict.SetValue("CREATE_BLANKING_VALUE", des.getCreateBlanking() ? "on" : "off");
    dict.SetValue("COMPRESS_BLANKING_VALUE", des.getBlankingXilinxCompression() ? "on" : "off");

    //Reference configuration
    dict.SetValue("COMPRESS_REFERENCE_VALUE", des.getReferenceXilinxCompression() ? "on" : "off");

    //Generate
    ctemplate::ExpandTemplate(TEMPLATE_DESIGN_FILE, ctemplate::DO_NOT_STRIP, &dict, &output);

    if (!output.empty()) {
        ofstream file(outputFile.c_str());
        file << output;
        file.close();

        result = true;
    }

    return result;
}

void GenDesignTCL::addStaticFilesToDict(ctemplate::TemplateDictionary &dict, Design &des) {
    ctemplate::TemplateDictionary *subdict;
    vector<vector<string> > staticFiles;

    staticFiles = des.getStaticFiles();

    for (size_t i = 0; i < staticFiles.size(); i++) {
        subdict = dict.AddSectionDictionary("STATIC_FILE");
        subdict->SetValue("STATIC_FILE_DIR", staticFiles[i][0]);
        subdict->SetValue("STATIC_FILE_NAME", staticFiles[i][1]);
        if (i == 0) {
            subdict->AddSectionDictionary("TOP_FILE");
        }
    }
}

void GenDesignTCL::addConstraintFilesToDict(ctemplate::TemplateDictionary &dict, Design &des) {
    ctemplate::TemplateDictionary *subdict;
    vector<vector<string> > constraintFiles;

    constraintFiles = des.getConstraintFiles();

    for (size_t i = 0; i < constraintFiles.size(); i++) {
        subdict = dict.AddSectionDictionary("CONSTRAINT_FILE");
        subdict->SetValue("CONSTRAINT_FILE_DIR", constraintFiles[i][0]);
        subdict->SetValue("CONSTRAINT_FILE_NAME", constraintFiles[i][1]);
    }
}

void GenDesignTCL::addPartitionsToDict(ctemplate::TemplateDictionary &dict, Design &des) {
    ctemplate::TemplateDictionary *subdict, *subsubdict;
    vector<ReconfigPartition> partitions;

    partitions = des.getPartitions();

    for (size_t i = 0; i < partitions.size(); i++) {
        vector<string> hwRes = partitions[i].getHwResources();

        subdict = dict.AddSectionDictionary("RECONFIG_PARTITION");
        //subdict->SetValue("PARTITION_ID", partitions[i].getPartitionID());
	subdict->SetValue("PARTITION_ID", partitions[i].getName());
        subdict->SetValue("PARTITION_NAME", partitions[i].getName());
        subdict->SetValue("PARTITION_SRC_DIR", partitions[i].getSrcDir());
        subdict->SetValue("PARTITION_INSTANCE_NAME", partitions[i].getInstanceName());
        subdict->SetValue("PARTITION_RESET_AFTER_RECONFIG", partitions[i].getResetAfterReconfig() ? "on" : "off");
        subdict->SetValue("PARTITION_SNAPPING_MODE", partitions[i].getSnappingMode() ? "on" : "off");

        for (size_t j = 0; j < hwRes.size(); j++) {
            subsubdict = subdict->AddSectionDictionary("PARTITION_HW_RESOURCE");
            subsubdict->SetValue("HW_RESOURCE", hwRes[j]);
        }
    }
}

void GenDesignTCL::addModulesToDict(ctemplate::TemplateDictionary &dict, Design &des) {
    ctemplate::TemplateDictionary *subdict, *subsubdict;
    vector<ReconfigModule> modules;

    modules = des.getModules();

    for (size_t i = 0; i < modules.size(); i++) {
        vector<vector<string> > files = modules[i].getFiles();

        subdict = dict.AddSectionDictionary("RECONFIG_MODULE");
        subdict->SetValue("FIXED_FILES_ID", modules[i].getModuleID());
        subdict->SetValue("RECONFIG_MODULE_PARTITION_ID", modules[i].getPartitionID());
        subdict->SetValue("RECONFIG_MODULE_NAME", modules[i].getName());

        for (size_t j = 0; j < files.size(); j++) {
            subsubdict = subdict->AddSectionDictionary("RECONFIG_MODULE_FILE");
            subsubdict->SetValue("FIXED_FILES_ID", modules[i].getModuleID());
            subsubdict->SetValue("RECONFIG_MODULE_FILE_DIR", files[j][0]);
            subsubdict->SetValue("RECONFIG_MODULE_FILE_NAME", files[j][1]);
        }
    }
}

void GenDesignTCL::addConfigurationsToDict(ctemplate::TemplateDictionary &dict, Design &des) {
    ctemplate::TemplateDictionary *subdict, *subsubdict;
    vector<Configuration> configs;

    configs = des.getConfigurations();

    for (size_t i = 0; i < configs.size(); i++) {
        vector<string> modulesIDs = configs[i].getModulesIDs();

        subdict = dict.AddSectionDictionary("CONFIGURATION");
        subdict->SetValue("CONFIG_ID", configs[i].getConfigID());
        subdict->SetValue("CONFIG_NAME", configs[i].getName());
        subdict->SetValue("CONFIG_BITSTREAM_COMPRESS", configs[i].getXilinxGeneralCompress() ? "on" : "off");

        for (size_t j = 0; j < modulesIDs.size(); j++) {
            ReconfigModule module = des.getModule(modulesIDs[j]);

            subsubdict = subdict->AddSectionDictionary("CONFIGURATION_ADD_MODULE");
            subsubdict->SetValue("CONFIG_ID", configs[i].getConfigID());
            subsubdict->SetValue("CONFIG_PARTITION_ID", module.getPartitionID());
            subsubdict->SetValue("CONFIG_RECONFIG_MODULE_NAME", module.getName());
        }
    }
}
