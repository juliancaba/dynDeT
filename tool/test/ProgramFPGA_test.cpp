#include "gtest/gtest.h"
#include "ProgramFPGA.h"

class ProgramFPGA_test : public ::testing::Test {
public:
    ProgramFPGA *progfpga;
    
    void SetUp() {
        //Initialization
        progfpga = new ProgramFPGA("Bitstreams/");
        progfpga->setFullBitstream("reference.bit");
        progfpga->addPartialBitstream("cfg0_rm_core_partial.bit", "0x000001");
        progfpga->addPartialBitstream("cfg1_rm_core_partial.bit", "0x100001");
        progfpga->addApplication("example/app.elf");
        progfpga->setWrapperHWPlatform("hw_platform_example.hdf");
        progfpga->setPart("example-part");
    }
    
    void TearDown() {
        //Code here will be called just after the test completes
        delete progfpga;
    }
};

TEST_F(ProgramFPGA_test, CheckAttrs) {
    ASSERT_EQ(progfpga->getBitstreamsDir(), "Bitstreams/");
    ASSERT_EQ(progfpga->getFullBitstream(), "reference.bit");
    ASSERT_EQ(progfpga->getPartialBitstreams().size(), 2);
    ASSERT_EQ(progfpga->getApplications().size(), 1);
    ASSERT_EQ(progfpga->getWrapperHWPlatform(), "hw_platform_example.hdf");
    ASSERT_EQ(progfpga->getPart(), "example-part");
}

TEST_F(ProgramFPGA_test, CheckCopyConstructor) {
    ProgramFPGA aux(*progfpga);
    
    ASSERT_EQ(aux.getBitstreamsDir(), progfpga->getBitstreamsDir());
    ASSERT_EQ(aux.getFullBitstream(), progfpga->getFullBitstream());
    ASSERT_EQ(aux.getPartialBitstreams().size(), progfpga->getPartialBitstreams().size());
    ASSERT_EQ(aux.getApplications().size(), progfpga->getApplications().size());
    ASSERT_EQ(aux.getWrapperHWPlatform(), progfpga->getWrapperHWPlatform());
    ASSERT_EQ(aux.getPart(), progfpga->getPart());
}

TEST_F(ProgramFPGA_test, CheckGenerateProgramTCL) {
    ASSERT_TRUE(progfpga->generateProgramTCL("tmp.tcl"));
}

TEST_F(ProgramFPGA_test, CheckGenerateXDMTCL) {
    ASSERT_TRUE(progfpga->generateXMDTCL("xmd_tmp.tcl"));
}

TEST_F(ProgramFPGA_test, CheckGenerateDeployMK) {
    ASSERT_TRUE(progfpga->generateDeployMK("deploy_tmp.mk"));
}

TEST_F(ProgramFPGA_test, CheckGenerateBootSDMK) {
    ASSERT_TRUE(progfpga->generateBootSDMK("boot_tmp.mk"));
}
