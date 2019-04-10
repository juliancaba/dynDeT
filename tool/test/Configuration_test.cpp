#include "gtest/gtest.h"
#include "ReconfigModule.h"
#include "Configuration.h"

class Configuration_test : public ::testing::Test {
public:
    ReconfigModule *module;
    Configuration *config;
    Configuration *config2;
    
    void SetUp() {
        //Initialization
        module = new ReconfigModule("fixed1", "fixed1_name", "rm_core");
        module->addFile("dir", "file_name");
        module->addFile("dir", "file_name2");
        
        config = new Configuration();
        config->setConfigID("cfg0ID");
        config->setName("cfg0Name");
        
        config2 = new Configuration ("cfg1ID", "cfg1Name");
        config2->addModule(module->getModuleID());
    }
    
    void TearDown() {
        //Code here will be called just after the test completes
        delete module;
        delete config;
        delete config2;
    }
};

TEST_F(Configuration_test, CheckIDs) {
    ASSERT_EQ(config->getConfigID(), "cfg0ID");
    ASSERT_EQ(config2->getConfigID(), "cfg1ID");
}

TEST_F(Configuration_test, CheckNames) {
    ASSERT_EQ(config->getName(), "cfg0Name");
    ASSERT_EQ(config2->getName(), "cfg1Name");
}

TEST_F(Configuration_test, CheckModules) {
    ASSERT_EQ(config->getModulesIDs().size(), 0);
    ASSERT_EQ(config2->getModulesIDs().size(), 1);
    
    ASSERT_EQ(config2->getModulesIDs()[0], module->getModuleID());
}

TEST_F(Configuration_test, CheckXilinxGeneralCompressOption) {
    ASSERT_FALSE(config->getXilinxGeneralCompress());
    ASSERT_FALSE(config2->getXilinxGeneralCompress());

    config->setXilinxGeneralCompress(true);
    
    ASSERT_TRUE(config->getXilinxGeneralCompress());
}

TEST_F(Configuration_test, CheckCopyConstructor) {
    Configuration aux(*config);
    
    ASSERT_EQ(aux.getConfigID(), "cfg0ID");
    ASSERT_EQ(aux.getName(), "cfg0Name");
    ASSERT_EQ(aux.getModulesIDs().size(), 0);
}

TEST_F(Configuration_test, CheckAssignmentOperator) {
    Configuration aux;
    
    aux = *config;
    
    ASSERT_EQ(aux.getConfigID(), "cfg0ID");
    ASSERT_EQ(aux.getName(), "cfg0Name");
    ASSERT_EQ(aux.getModulesIDs().size(), 0);
}
