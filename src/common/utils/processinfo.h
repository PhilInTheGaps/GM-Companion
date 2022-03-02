#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>
#include <QString>

class ProcessInfo
{
public:
    static auto isProcessRunning(const QString &procName) -> bool;

private:
    #ifdef Q_OS_MACOS
    static auto isProcessRunningMac(const QString &procName) -> bool;
    #elif defined Q_OS_UNIX
    static auto isProcessRunningUnix(const QString &procName) -> bool;
    #elif defined Q_OS_WIN
    static auto isProcessRunningWin(const QString &name) -> bool;
    #endif
};

#endif // PROCESSINFO_H
