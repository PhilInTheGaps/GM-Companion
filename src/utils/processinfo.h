#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>

class ProcessInfo
{
public:
    ProcessInfo(){}

    int getProcIdByName(std::string procName);
};

#endif // PROCESSINFO_H
