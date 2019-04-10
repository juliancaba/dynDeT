#include "gtest/gtest.h"
#include "Site.h"

class Site_test : public ::testing::Test {
public:
    Site *site1, *site2;
    
    void SetUp() {
        site1 = new Site();
        
        site2 = new Site("EXAMPLE_X0Y0", "SLICEXAMPLE", "CLK_0", 0, 5);
    }
    
    void TearDown() {
        delete site1;
        delete site2;
    }
};

TEST_F(Site_test, CheckSetAttrs) {
    site1->setName(site2->getName());
    site1->setType(site2->getType());
    site1->setClockRegion(site2->getClockRegion());
    site1->setRPMX(site2->getRPMX());
    site1->setRPMY(site2->getRPMY());
    
    ASSERT_EQ(site1->getName(), site2->getName());
    ASSERT_EQ(site1->getType(), site2->getType());
    ASSERT_EQ(site1->getClockRegion(), site2->getClockRegion());
    ASSERT_EQ(site1->getRPMX(), site2->getRPMX());
    ASSERT_EQ(site1->getRPMY(), site2->getRPMY());
}

TEST_F(Site_test, CheckCopyConstructor) {
    Site newSite(*site2);
    
    ASSERT_EQ(newSite.getName(), site2->getName());
    ASSERT_EQ(newSite.getType(), site2->getType());
    ASSERT_EQ(newSite.getClockRegion(), site2->getClockRegion());
    ASSERT_EQ(newSite.getRPMX(), site2->getRPMX());
    ASSERT_EQ(newSite.getRPMY(), site2->getRPMY());
}
