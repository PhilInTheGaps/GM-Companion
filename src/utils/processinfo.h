#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>

class ProcessInfo
{
public:
    ProcessInfo(){}

    static bool isProcessRunning(std::string procName);
};

#endif // PROCESSINFO_H
