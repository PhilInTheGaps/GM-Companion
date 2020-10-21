#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>
#include <QString>

class ProcessInfo
{
public:
    static bool isProcessRunning(std::string procName);

private:
    #ifdef Q_OS_WIN
    static bool isProcessRunningWin(const QString &name);
    #endif
};

#endif // PROCESSINFO_H
