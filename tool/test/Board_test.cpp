#include "gtest/gtest.h"
#include "Board.h"

class Board_test : public ::testing::Test {
public:
    Board *board1, *board2;
    
    void SetUp() {
        board1 = new Board();
        
        board2 = new Board("em:avnet:example:asdf", 500, 600);
    }
    
    void TearDown() {
        delete board1;
        delete board2;
    }
};


TEST_F(Board_test, CheckSetAttrs) {
    board2->setName("example");
    board2->setRows(100);
    board2->setCols(50);
    
    ASSERT_EQ(board2->getName(), "example");
    ASSERT_EQ(board2->getRows(), 100);
    ASSERT_EQ(board2->getCols(), 50);
    
    Site site1("EXAMPLE_X0Y0", "SLICEXAMPLE", "CLK_0", 0, 5);
    Site site2("EXAMPLE_X10Y10", "SLIMEXAMPLE", "CLK_1", 10, 15);
    
    board2->addSite(site1);
    board2->addSite(site2);
    
    ASSERT_EQ(board2->getNumOfSites(), 2);
    ASSERT_EQ(board2->getSiteAt(0).getType(), "SLICEXAMPLE");
    ASSERT_EQ(board2->getSites()[1].getType(), "SLIMEXAMPLE");
}

TEST_F(Board_test, CheckLoadFromXMLAndCopyConstructor) {
    ASSERT_TRUE(board1->loadFromXML("xml/em.avnet.com:zed:part0:1.2.xml"));
    
    ASSERT_EQ(board1->getNumOfSites(), 15116);
    ASSERT_EQ(board1->getRows(), 299);
    ASSERT_EQ(board1->getCols(), 308);
    ASSERT_EQ(board1->getName(), "em.avnet.com:zed:part0:1.2");
    
    Board newBoard(*board1);
    
    ASSERT_EQ(newBoard.getName(), board1->getName());
    ASSERT_EQ(newBoard.getRows(), board1->getRows());
    ASSERT_EQ(newBoard.getCols(), board1->getCols());
    ASSERT_EQ(newBoard.getNumOfSites(), board1->getNumOfSites());
    ASSERT_EQ(newBoard.getSiteAt(0).getType(), board1->getSiteAt(0).getType());
}
