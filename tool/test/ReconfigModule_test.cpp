#include "gtest/gtest.h"
#include "ReconfigPartition.h"
#include "ReconfigModule.h"

class ReconfigModule_test : public ::testing::Test {
public:
    ReconfigPartition *partition;
    ReconfigModule *module;
    ReconfigModule *module2;

    void SetUp() {
        //Initialization
        partition = new ReconfigPartition("rm_core", "wrapperTDD", "wrapperTDD_1", "src/", true, true);
        partition->addHwResource("SLICE_X28Y3:SLICE_X47Y47");
        
        module = new ReconfigModule();
        module->setModuleID("fixed1");
        module->setName("fixed1_name");
        module->setPartitionID(partition->getPartitionID());
        
        module2 = new ReconfigModule("fixed2", "fixed2_name", partition->getPartitionID());
        
        module->addFile("dir", "nameFile");
        module->addFile("dir", "nameFile2");
        module2->addFile("dir2", "nameFile3");
    }
    
    void TearDown() {
        //Code here will be called just after the test completes
        delete partition;
        
        delete module;
        delete module2;
    }
};


TEST_F(ReconfigModule_test, CheckIDs) {
    ASSERT_EQ(module->getModuleID(), "fixed1");
    ASSERT_EQ(module2->getModuleID(), "fixed2");
}

TEST_F(ReconfigModule_test, CheckNames) {
    ASSERT_EQ(module->getName(), "fixed1_name");
    ASSERT_EQ(module2->getName(), "fixed2_name");
}

TEST_F(ReconfigModule_test, CheckPartitions) {
    ASSERT_EQ(module->getPartitionID(), partition->getPartitionID());
    ASSERT_EQ(module2->getPartitionID(), partition->getPartitionID());
}

TEST_F(ReconfigModule_test, CheckAddFiles) {
    int size;
    
    size = module->getFiles().size();
    ASSERT_EQ(size, 2);
    
    size = module2->getFiles().size();
    ASSERT_EQ(size, 1);
    
    ASSERT_EQ(module->getFiles()[0][0], "dir");
    ASSERT_EQ(module->getFiles()[1][1], "nameFile2");
    ASSERT_EQ(module2->getFiles()[0][1], "nameFile3");
}

TEST_F(ReconfigModule_test, CheckCopyConstructor) {
    ReconfigModule moduleAux(*module);
    
    ASSERT_EQ(moduleAux.getModuleID(), "fixed1");
    ASSERT_EQ(moduleAux.getName(), "fixed1_name");
    ASSERT_EQ(moduleAux.getPartitionID(), partition->getPartitionID());
    ASSERT_EQ(moduleAux.getFiles().size(), 2);
    ASSERT_EQ(moduleAux.getFiles()[0][0], "dir");
}

TEST_F(ReconfigModule_test, CheckAssignmentOperator) {
    ReconfigModule moduleAux;
    
    moduleAux = *module;
    
    ASSERT_EQ(moduleAux.getModuleID(), "fixed1");
    ASSERT_EQ(moduleAux.getName(), "fixed1_name");
    ASSERT_EQ(moduleAux.getPartitionID(), partition->getPartitionID());
    ASSERT_EQ(moduleAux.getFiles().size(), 2);
    ASSERT_EQ(moduleAux.getFiles()[0][0], "dir");
}

