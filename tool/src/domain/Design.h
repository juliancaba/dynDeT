#ifndef SRC_DESIGN_H_
#define SRC_DESIGN_H_

#include "ReconfigPartition.h"
#include "ReconfigModule.h"
#include "Configuration.h"


class Design {
private:
    std::string part;
    std::string board;
    std::string projectName;
    std::string designName;
    std::string ipDir;
    std::string blockDesignTCL;
    std::vector<std::vector<std::string> > staticFiles;
    std::vector<std::vector<std::string> > constraintFiles;
    std::vector<ReconfigPartition> partitions;
    std::vector<ReconfigModule> modules;
    std::vector<Configuration> configurations;

    //Create blanking configuration
    bool createBlanking;
    bool referenceXilinxCompression;
    bool blankingXilinxCompression;

public:
    Design();
    Design(const Design &d);
    Design& operator=(const Design &d);

    bool generateTCL(std::string outputFile);
    bool generateXML(std::string outputFile);
    bool copyFile(std::string outputFile);
    bool loadFromXML(std::string inputFile);

    void setPart(std::string p);
    void setBoard(std::string b);
    void setProjectName(std::string prjname);
    void setDesignName(std::string dname);
    void setIPDir(std::string dir);
    void setBlockDesignTCL(std::string blockdes);
    void addStaticFile(std::string dir, std::string name);
    void addConstraintFile(std::string dir, std::string name);
    void addPartition(ReconfigPartition &partition);
    bool addModule(ReconfigModule &module);
    bool addConfiguration(Configuration &config);
    void setCreateBlanking(bool val);
    void setReferenceXilinxCompression(bool val);
    void setBlankingXilinxCompression(bool val);
    bool setXilinxCompression(std::string configID, bool value);


    std::string getPart() const;
    std::string getBoard() const;
    std::string getProjectName() const;
    std::string getDesignName() const;
    std::string getIPDir() const;
    std::string getBlockDesignTCL() const;
    std::vector<std::vector<std::string> > getStaticFiles() const;
    std::vector<std::vector<std::string> > getConstraintFiles() const;
    ReconfigPartition getPartition(std::string id) const;
    ReconfigModule getModule(std::string id) const;
    Configuration getConfiguration(std::string id) const;
    std::vector<ReconfigPartition> getPartitions() const;
    std::vector<ReconfigModule> getModules() const;
    std::vector<Configuration> getConfigurations() const;
    bool getCreateBlanking() const;
    bool getReferenceXilinxCompression() const;
    bool getBlankingXilinxCompression() const;
};


#endif
