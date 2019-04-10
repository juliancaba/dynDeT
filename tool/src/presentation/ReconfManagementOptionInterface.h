#ifndef RECONFMANAGEMENTOPTIONINTERFACE_H
#define RECONFMANAGEMENTOPTIONINTERFACE_H

//"Interface class" (abstract class) for all the possible ways of manage the reconfiguration

class ReconfManagementOptionInterface {
public:
    virtual ~ReconfManagementOptionInterface() {}
    virtual bool getProgrammingNow() = 0;
};

#endif
