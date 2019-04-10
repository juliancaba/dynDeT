#include "gtest/gtest.h"
#include "ReconfigPartition.h"

class ReconfigPartition_test : public ::testing::Test {
public:
    ReconfigPartition *partition;
    ReconfigPartition *partitionWithHwRes;
    
    void SetUp() {
        //Initialization
        partition = new ReconfigPartition();
        partition->setPartitionID("rm_core");
        partition->setName("wrapperTDD");
        partition->setInstanceName("wrapperTDD_1");
        partition->setSrcDir("src/");
        
        
        partitionWithHwRes = new ReconfigPartition("rm_core2", "wrapperTDD2", "wrapperTDD_2", "src/2/", true, true);
        partitionWithHwRes->addHwResource("SLICE_X28Y3:SLICE_X47Y47");
        partitionWithHwRes->addHwResource("DSP48_X2Y2:DSP48_X2Y17");
        partitionWithHwRes->addHwResource("RAMB18_X2Y2:RAMB18_X2Y17");
    }
    
    void TearDown() {
        //Code here will be called just after the test completes
        delete partition;
        delete partitionWithHwRes;
    }
};

TEST_F(ReconfigPartition_test, CheckIDs) {
    ASSERT_EQ(partition->getPartitionID(), "rm_core");
    ASSERT_EQ(partitionWithHwRes->getPartitionID(), "rm_core2");
}

TEST_F(ReconfigPartition_test, CheckNames) {
    ASSERT_EQ(partition->getName(), "wrapperTDD");
    ASSERT_EQ(partitionWithHwRes->getName(), "wrapperTDD2");
}

TEST_F(ReconfigPartition_test, CheckInstanceNames) {
    ASSERT_EQ(partition->getInstanceName(), "wrapperTDD_1");
    ASSERT_EQ(partitionWithHwRes->getInstanceName(), "wrapperTDD_2");
}

TEST_F(ReconfigPartition_test, CheckSrcDir) {
    ASSERT_EQ(partition->getSrcDir(), "src/");
    ASSERT_EQ(partitionWithHwRes->getSrcDir(), "src/2/");
}

TEST_F(ReconfigPartition_test, CheckHWResSize) {
    int size;
    
    size = partition->getHwResources().size();
    ASSERT_EQ(size, 0);
    
    size = partitionWithHwRes->getHwResources().size();
    ASSERT_EQ(size, 3);
}

TEST_F(ReconfigPartition_test, CheckHWRes) {
    ASSERT_EQ(partitionWithHwRes->getHwResources()[0], "SLICE_X28Y3:SLICE_X47Y47");
    ASSERT_EQ(partitionWithHwRes->getHwResources()[1], "DSP48_X2Y2:DSP48_X2Y17");
    ASSERT_EQ(partitionWithHwRes->getHwResources()[2], "RAMB18_X2Y2:RAMB18_X2Y17");
}

TEST_F(ReconfigPartition_test, CheckProperties) {
    ASSERT_TRUE(partitionWithHwRes->getResetAfterReconfig());
    ASSERT_TRUE(partitionWithHwRes->getSnappingMode());
    
    partitionWithHwRes->setResetAfterReconfig(false);
    partitionWithHwRes->setSnappingMode(false);
    
    ASSERT_FALSE(partitionWithHwRes->getResetAfterReconfig());
    ASSERT_FALSE(partitionWithHwRes->getSnappingMode());
    
    ASSERT_FALSE(partition->getResetAfterReconfig());
    ASSERT_FALSE(partition->getSnappingMode());
}

TEST_F(ReconfigPartition_test, CheckCopyConstructor) {
    ReconfigPartition partitionAux(*partitionWithHwRes);
    
    ASSERT_EQ(partitionAux.getPartitionID(), "rm_core2");
    ASSERT_EQ(partitionAux.getName(), "wrapperTDD2");
    ASSERT_EQ(partitionAux.getInstanceName(), "wrapperTDD_2");
    ASSERT_EQ(partitionAux.getSrcDir(), "src/2/");
    ASSERT_EQ(partitionAux.getHwResources().size(), 3);
    ASSERT_EQ(partitionAux.getHwResources()[0], "SLICE_X28Y3:SLICE_X47Y47");
}

TEST_F(ReconfigPartition_test, CheckAssignmentOperator) {
    ReconfigPartition partitionAux;
    
    partitionAux = *partitionWithHwRes;
    
    ASSERT_EQ(partitionAux.getPartitionID(), "rm_core2");
    ASSERT_EQ(partitionAux.getName(), "wrapperTDD2");
    ASSERT_EQ(partitionAux.getInstanceName(), "wrapperTDD_2");
    ASSERT_EQ(partitionAux.getSrcDir(), "src/2/");
    ASSERT_EQ(partitionAux.getHwResources().size(), 3);
    ASSERT_EQ(partitionAux.getHwResources()[0], "SLICE_X28Y3:SLICE_X47Y47");
}

