#ifndef SRC_RECONFIGPARTITION_H_
#define SRC_RECONFIGPARTITION_H_

#include <vector>
#include <string>

class ReconfigPartition {
private:
    std::string partitionID;
    std::string name;
    std::string instanceName;
    std::string srcDir;
    bool resetAfterReconfig;
    bool snappingMode;
    std::vector<std::string> hwResources;

public:
    ReconfigPartition();
    ReconfigPartition(std::string pID, std::string n, std::string instN, std::string srcD, bool reset, bool snapping);
    ReconfigPartition(const ReconfigPartition &p);
    ReconfigPartition& operator=(const ReconfigPartition &p);

    void addHwResource(std::string hwres);

    std::string getPartitionID() const;
    std::string getName() const;
    std::string getInstanceName() const;
    std::string getSrcDir() const;
    bool getResetAfterReconfig() const;
    bool getSnappingMode() const;
    std::vector<std::string> getHwResources() const;

    void setPartitionID(std::string id);
    void setName(std::string n);
    void setInstanceName(std::string instN);
    void setSrcDir(std::string srcD);
    void setResetAfterReconfig(bool reset);
    void setSnappingMode(bool snapping);
};


#endif
