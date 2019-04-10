#include "gtest/gtest.h"
#include "Design.h"

class Design_test : public ::testing::Test {
public:
    Design *design;
    ReconfigPartition *partition0, *partition1;
    ReconfigModule *module0, *module1, *module2, *module3, *moduleE;
    Configuration *config0, *config1, *configE;
    
    void SetUp() {
        design = new Design();
        
        //Board data
	    design->setPart("xc7z020clg484-1");
	    design->setBoard("em.avnet.com:zed:part0:1.2");

	    //Vivado project data
	    design->setProjectName("tmp");
	    design->setDesignName("ps_system");
	    design->setIPDir("ip_repo");
	    design->setBlockDesignTCL("src/design/design_zipFactory_outDPR_2015_4.tcl");

	    //Static files
	    design->addStaticFile("src/top/", "top.vhd");

	    //Constraint files
	    design->addConstraintFile("src/xdc/", "topZedboard.xdc");

	    //Create partitio0
	    partition0 = new ReconfigPartition("rm_core", "wrapperTDD", "wrapperTDD_1", "", true, true);
	    partition0->addHwResource("SLICE_X28Y3:SLICE_X47Y47");
	    partition0->addHwResource("DSP48_X2Y2:DSP48_X2Y17");
	    partition0->addHwResource("RAMB18_X2Y2:RAMB18_X2Y17");
	    partition0->addHwResource("RAMB36_X2Y1:RAMB36_X2Y8");

	    //Partition1
	    partition1 = new ReconfigPartition("rm_core2", "wrapperTDDp2", "wrapperTDD_2", "", true, true);
	    partition1->addHwResource("SLICE_X78Y3:SLICE_X97Y47");
	    partition1->addHwResource("DSP48_X3Y2:DSP48_X3Y17");
	    partition1->addHwResource("RAMB18_X4Y2:RAMB18_X4Y17");
	    partition1->addHwResource("RAMB36_X4Y2:RAMB36_X4Y8");

	    //Module0
	    module0 = new ReconfigModule("fixed1Files", "fixed1", partition0->getPartitionID());
	    module0->addFile("src/dynModules/chrono/", "wrapperTDD.vhd");

	    //Module1
	    module1 = new ReconfigModule("fixed2Files", "fixed2", partition0->getPartitionID());
	    module1->addFile("src/dynModules/l2norm_add_mult/", "fifoAux_32b.dcp");
	    module1->addFile("src/dynModules/l2norm_add_mult/", "add_fp.dcp");
	    module1->addFile("src/dynModules/l2norm_add_mult/", "mult_fp.dcp");
	    module1->addFile("src/dynModules/l2norm_add_mult/", "recip_fp.dcp");
	    module1->addFile("src/dynModules/l2norm_add_mult/", "sqrt_fp.dcp");
	    module1->addFile("src/dynModules/l2norm_add_mult/", "l2norm_algorithm.vhd");
	    module1->addFile("src/dynModules/l2norm_add_mult/", "l2norm_protocol.vhd");
	    module1->addFile("src/dynModules/l2norm_add_mult/", "wrapperTDD.vhd");

	    //Module2
	    module2 = new ReconfigModule("fixed3Files", "fixed3", partition1->getPartitionID());
	    module2->addFile("src/dynModules/dummy1/", "wrapperTDDp2.vhd");

	    //Module 3
	    module3 = new ReconfigModule("fixed4Files", "fixed4", partition0->getPartitionID());
	    module3->addFile("src/dynModules/l2hys_stream/", "fifoAux_32b.dcp");
	    module3->addFile("src/dynModules/l2hys_stream/", "mult_add_fp_stream.dcp");
	    module3->addFile("src/dynModules/l2hys_stream/", "recip_fp.dcp");
	    module3->addFile("src/dynModules/l2hys_stream/", "sqrt_fp.dcp");
	    module3->addFile("src/dynModules/l2hys_stream/", "l2hys_multaddstream_algorithm.vhd");
	    module3->addFile("src/dynModules/l2hys_stream/", "l2hys_multaddstream_protocol.vhd");
	    module3->addFile("src/dynModules/l2hys_stream/", "wrapperTDD.vhd");
	    
	    //Module (error)
	    moduleE = new ReconfigModule("error", "error", "<partition that doesnt exists>");
	    moduleE->addFile("asdf", "asdf");

	    //Config0
	    config0 = new Configuration("cfg0", "cfg0");
	    config0->addModule(module1->getModuleID());
	    config0->addModule(module2->getModuleID());
	    config0->setXilinxGeneralCompress(true);

	    //Config1
	    config1 = new Configuration("cfg1", "cfg1");
	    config1->addModule(module3->getModuleID());
	    config1->addModule(module2->getModuleID());
	    config1->setXilinxGeneralCompress(true);
	    
	    //Config (error)
	    configE = new Configuration("asdf", "asdf");
	    configE->addModule(moduleE->getModuleID());
	    configE->addModule(module2->getModuleID());
    }
    
    void TearDown() {
        delete design;
        delete partition0;
        delete partition1;
        delete module0;
        delete module1;
        delete module2;
        delete module3;
        delete moduleE;
        delete config0;
        delete config1;
        delete configE;
    }
};


TEST_F(Design_test, CheckBoardDataAndVivadoProjectData) {
    ASSERT_EQ(design->getPart(), "xc7z020clg484-1");
    ASSERT_EQ(design->getBoard(), "em.avnet.com:zed:part0:1.2");
    
    ASSERT_EQ(design->getProjectName(), "tmp");
    ASSERT_EQ(design->getDesignName(), "ps_system");
    ASSERT_EQ(design->getIPDir(), "ip_repo");
    ASSERT_EQ(design->getBlockDesignTCL(), "src/design/design_zipFactory_outDPR_2015_4.tcl");
}

TEST_F(Design_test, CheckStaticFiles) {
    ASSERT_EQ(design->getStaticFiles().size(), 1);
    
    ASSERT_EQ(design->getStaticFiles()[0][0], "src/top/");
    ASSERT_EQ(design->getStaticFiles()[0][1], "top.vhd");
}

TEST_F(Design_test, CheckConstraintFiles) {
    ASSERT_EQ(design->getConstraintFiles().size(), 1);
    
    ASSERT_EQ(design->getConstraintFiles()[0][0], "src/xdc/");
    ASSERT_EQ(design->getConstraintFiles()[0][1], "topZedboard.xdc");
}

TEST_F(Design_test, CheckAddPartitions) {
    ASSERT_EQ(design->getPartitions().size(), 0);
    
    design->addPartition(*partition0);
    design->addPartition(*partition1);
    
    ASSERT_EQ(design->getPartitions().size(), 2);
    
    ASSERT_EQ(design->getPartitions()[0].getPartitionID(), partition0->getPartitionID());
    ASSERT_EQ(design->getPartitions()[1].getPartitionID(), partition1->getPartitionID());
}

TEST_F(Design_test, CheckAddModules) {
    design->addPartition(*partition0);
    design->addPartition(*partition1);
    
    ASSERT_EQ(design->getModules().size(), 0);
    
    ASSERT_TRUE(design->addModule(*module0));
    ASSERT_TRUE(design->addModule(*module1));
    ASSERT_FALSE(design->addModule(*moduleE));
    ASSERT_TRUE(design->addModule(*module2));
    ASSERT_TRUE(design->addModule(*module3));
    
    ASSERT_EQ(design->getModules().size(), 4);
    
    ASSERT_EQ(design->getModules()[0].getPartitionID(), partition0->getPartitionID());
    ASSERT_EQ(design->getModules()[1].getPartitionID(), partition0->getPartitionID());
    ASSERT_EQ(design->getModules()[2].getPartitionID(), partition1->getPartitionID());
    ASSERT_EQ(design->getModules()[3].getPartitionID(), partition0->getPartitionID());
}

TEST_F(Design_test, CheckGetOneObjectByID) {
    design->addPartition(*partition0);
    design->addPartition(*partition1);
    design->addModule(*module0);
    design->addModule(*module1);
    design->addModule(*module2);
    design->addModule(*module3);
    design->addConfiguration(*config0);
    design->addConfiguration(*config1);
    
    ASSERT_EQ(design->getPartition(partition0->getPartitionID()).getName(), partition0->getName());
    ASSERT_EQ(design->getModule(module2->getModuleID()).getName(), module2->getName());
    ASSERT_EQ(design->getConfiguration(config1->getConfigID()).getName(), config1->getName());
}


TEST_F(Design_test, CheckAddConfigurations) {
    design->addPartition(*partition0);
    design->addPartition(*partition1);
    design->addModule(*module0);
    design->addModule(*module1);
    design->addModule(*module2);
    design->addModule(*module3);
    
    
    ASSERT_EQ(design->getConfigurations().size(), 0);
    
    ASSERT_TRUE(design->addConfiguration(*config0));
    ASSERT_FALSE(design->addConfiguration(*configE));
    ASSERT_TRUE(design->addConfiguration(*config1));
    
    ASSERT_EQ(design->getConfigurations().size(), 2);
    
    ASSERT_EQ(design->getConfigurations()[0].getModulesIDs()[0], module1->getModuleID());
    ASSERT_EQ(design->getConfigurations()[0].getModulesIDs()[1], module2->getModuleID());
    ASSERT_EQ(design->getConfigurations()[1].getModulesIDs()[0], module3->getModuleID());
    ASSERT_EQ(design->getConfigurations()[1].getModulesIDs()[1], module2->getModuleID());
}

TEST_F(Design_test, CheckGenerateTCL) {
    design->addPartition(*partition0);
    design->addPartition(*partition1);
    design->addModule(*module0);
    design->addModule(*module1);
    design->addModule(*module2);
    design->addModule(*module3);
    design->addConfiguration(*config0);
    design->addConfiguration(*config1);
    
    ASSERT_TRUE(design->generateTCL("tmp.tcl"));
}

TEST_F(Design_test, CheckGenerateXML_LoadFromXML) {
    design->addPartition(*partition0);
    design->addPartition(*partition1);
    design->addModule(*module0);
    design->addModule(*module1);
    design->addModule(*module2);
    design->addModule(*module3);
    design->addConfiguration(*config0);
    design->addConfiguration(*config1);
    
    ASSERT_TRUE(design->generateXML("tmp.xml"));
    
    //Load the saved design in a new design object
    Design newDesign;
    std::vector<std::vector<std::string> > staticFiles, newStaticFiles;
    std::vector<std::vector<std::string> > constraintFiles, newConstraintFiles;
    
    ASSERT_TRUE(newDesign.loadFromXML("tmp.xml"));
    
    ASSERT_EQ(newDesign.getPart(), design->getPart());
    ASSERT_EQ(newDesign.getBoard(), design->getBoard());
    ASSERT_EQ(newDesign.getProjectName(), design->getProjectName());
    ASSERT_EQ(newDesign.getDesignName(), design->getDesignName());
    ASSERT_EQ(newDesign.getIPDir(), design->getIPDir());
    ASSERT_EQ(newDesign.getBlockDesignTCL(), design->getBlockDesignTCL());
    
    staticFiles = design->getStaticFiles();
    newStaticFiles = newDesign.getStaticFiles();
    ASSERT_EQ(staticFiles.size(), newStaticFiles.size());
    
    for (unsigned int i = 0; i < staticFiles.size(); i++) {
        ASSERT_EQ(staticFiles[i][0], newStaticFiles[i][0]);
        ASSERT_EQ(staticFiles[i][1], newStaticFiles[i][1]);
    }
    
    constraintFiles = design->getConstraintFiles();
    newConstraintFiles = newDesign.getConstraintFiles();
    
    for (unsigned int i = 0; i < constraintFiles.size(); i++) {
        ASSERT_EQ(constraintFiles[i][0], newConstraintFiles[i][0]);
        ASSERT_EQ(constraintFiles[i][1], newConstraintFiles[i][1]);
    }
    
    ASSERT_EQ(newDesign.getPartitions().size(), 2);
    
    ASSERT_EQ(newDesign.getPartitions()[0].getPartitionID(), partition0->getPartitionID());
    ASSERT_EQ(newDesign.getPartitions()[1].getPartitionID(), partition1->getPartitionID());
    
    ASSERT_EQ(newDesign.getModules().size(), 4);
    
    ASSERT_EQ(newDesign.getModules()[0].getModuleID(), module0->getModuleID());
    ASSERT_EQ(newDesign.getModules()[1].getModuleID(), module1->getModuleID());
    ASSERT_EQ(newDesign.getModules()[2].getModuleID(), module2->getModuleID());
    ASSERT_EQ(newDesign.getModules()[3].getModuleID(), module3->getModuleID());
    
    ASSERT_EQ(newDesign.getConfigurations().size(), 2);
    
    ASSERT_EQ(newDesign.getConfigurations()[0].getConfigID(), config0->getConfigID());
    ASSERT_EQ(newDesign.getConfigurations()[1].getConfigID(), config1->getConfigID());
}

TEST_F(Design_test, CheckCopyConstructor) {
    Design aux(*design);
    
    ASSERT_EQ(aux.getPart(), design->getPart());
    ASSERT_EQ(aux.getBoard(), design->getBoard());
    ASSERT_EQ(aux.getProjectName(), design->getProjectName());
    ASSERT_EQ(aux.getDesignName(), design->getDesignName());
    ASSERT_EQ(aux.getBlockDesignTCL(), design->getBlockDesignTCL());
    ASSERT_EQ(aux.getStaticFiles().size(), design->getStaticFiles().size());
    ASSERT_EQ(aux.getConstraintFiles().size(), design->getConstraintFiles().size());
    ASSERT_EQ(aux.getPartitions().size(), design->getPartitions().size());
    ASSERT_EQ(aux.getModules().size(), design->getModules().size());
    ASSERT_EQ(aux.getConfigurations().size(), design->getConfigurations().size());
}

TEST_F(Design_test, CheckAssignmentOperator) {
    Design aux;
    
    aux = *design;
    
    ASSERT_EQ(aux.getPart(), design->getPart());
    ASSERT_EQ(aux.getBoard(), design->getBoard());
    ASSERT_EQ(aux.getProjectName(), design->getProjectName());
    ASSERT_EQ(aux.getDesignName(), design->getDesignName());
    ASSERT_EQ(aux.getBlockDesignTCL(), design->getBlockDesignTCL());
    ASSERT_EQ(aux.getStaticFiles().size(), design->getStaticFiles().size());
    ASSERT_EQ(aux.getConstraintFiles().size(), design->getConstraintFiles().size());
    ASSERT_EQ(aux.getPartitions().size(), design->getPartitions().size());
    ASSERT_EQ(aux.getModules().size(), design->getModules().size());
    ASSERT_EQ(aux.getConfigurations().size(), design->getConfigurations().size());
}

TEST_F(Design_test, CheckBlankingAndCompressionValues) {
    ASSERT_FALSE(design->getCreateBlanking());
    ASSERT_FALSE(design->getReferenceXilinxCompression());
    ASSERT_FALSE(design->getBlankingXilinxCompression());
    
    design->setCreateBlanking(true);
    design->setReferenceXilinxCompression(true);
    design->setBlankingXilinxCompression(true);
    
    ASSERT_TRUE(design->getCreateBlanking());
    ASSERT_TRUE(design->getReferenceXilinxCompression());
    ASSERT_TRUE(design->getBlankingXilinxCompression());
}

