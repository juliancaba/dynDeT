#ifndef SRC_CONFIGURATION_H_
#define SRC_CONFIGURATION_H_

#include <string>
#include <vector>

class Configuration {
private:
    std::string configID;
    std::string name;
    std::vector<std::string> modulesIDs;
    bool xilinxGeneralCompress;

public:
    Configuration();
    Configuration(std::string cID, std::string n);
    Configuration(const Configuration &c);
    Configuration& operator=(const Configuration &c);

    void addModule(std::string moduleID);

    std::string getConfigID() const;
    std::string getName() const;
    std::vector<std::string> getModulesIDs() const;
    bool getXilinxGeneralCompress() const;

    void setConfigID(std::string id);
    void setName(std::string n);
    void setXilinxGeneralCompress(bool value);
};



#endif
