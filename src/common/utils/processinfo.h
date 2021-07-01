#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>
#include <QString>

class ProcessInfo
{
public:
    static bool isProcessRunning(const QString &procName);

private:
    #ifdef Q_OS_MACOS
    static bool isProcessRunningMac(const char *procName);
    #elif defined Q_OS_UNIX
    static bool isProcessRunningUnix(std::string procName);
    #elif defined Q_OS_WIN
    static bool isProcessRunningWin(const QString &name);
    #endif
};

#endif // PROCESSINFO_H
