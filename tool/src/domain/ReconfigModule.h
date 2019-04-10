#ifndef SRC_RECONFIGMODULE_H_
#define SRC_RECONFIGMODULE_H_

#include <string>
#include <vector>

class ReconfigModule {
private:
    std::string moduleID;
    std::string name;
    std::string partitionID;
    std::vector<std::vector<std::string> > files;

public:
    ReconfigModule();
    ReconfigModule(std::string mID, std::string n, std::string partID);
    ReconfigModule(const ReconfigModule &m);
    ReconfigModule& operator=(const ReconfigModule &m);

    void addFile(std::string dir, std::string fileName);

    std::string getModuleID() const;
    std::string getName() const;
    std::string getPartitionID() const;
    std::vector<std::vector<std::string> > getFiles() const;

    void setModuleID(std::string id);
    void setName(std::string n);
    void setPartitionID(std::string partID);
};


#endif
