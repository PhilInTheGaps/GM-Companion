#include "processinfo.h"
#include <QtGlobal>
#include <QDebug>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#ifdef Q_OS_MACOS
#include <libproc.h>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <QStringList>
#endif

using namespace std;

bool ProcessInfo::isProcessRunning(const QString &procName)
{
    #ifdef Q_OS_MACOS

    return isProcessRunningMac(procName.toUtf8());

    #elif defined Q_OS_UNIX

    return isProcessRunningUnix(procName.toStdString());

    #elif defined Q_OS_WIN
    // https://stackoverflow.com/a/57164620

    for (const auto &name : { procName, QString("%1%2").arg(procName, ".exe") })
    {
        if (isProcessRunningWin(name)) return true;
    }

    #endif

    qWarning() << "Error: Method isProcessRunning() is not defined for this operating system!";

    return false;
}

#ifdef Q_OS_MACOS
auto ProcessInfo::isProcessRunningMac(const char *procName) -> bool
{
    // https://stackoverflow.com/questions/49506579/how-to-find-the-pid-of-any-process-in-mac-osx-c

    pid_t pids[2048];
    int bytes = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    int n_proc = bytes / sizeof(pids[0]);

    for (int i = 0; i < n_proc; i++)
    {
        struct proc_bsdinfo proc;
        int st = proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0, &proc, PROC_PIDTBSDINFO_SIZE);

        if (st == PROC_PIDTBSDINFO_SIZE)
        {
            if (strcmp(procName, proc.pbi_name) == 0)
            {
                return true;
            }
        }
    }

    return false;
}
#elif defined Q_OS_UNIX
auto ProcessInfo::isProcessRunningUnix(std::string procName) -> bool
{
    // https://proswdev.blogspot.com/2012/02/get-process-id-by-name-in-linux-using-c.html
    // Open the /proc directory
    DIR *dp = opendir("/proc");
    bool isRunning = false;

    if (dp != nullptr)
    {
        // Enumerate all entries in directory until process found
        struct dirent *dirp;

        int pid = -1;
        while (pid < 0 && (dirp = readdir(dp)))
        {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);

            if (id > 0)
            {
                // Read contents of virtual /proc/{pid}/cmdline file
                string   cmdPath = string("/proc/") + dirp->d_name + "/cmdline";
                ifstream cmdFile(cmdPath.c_str());
                string   cmdLine;
                getline(cmdFile, cmdLine);

                if (!cmdLine.empty())
                {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');

                    if (pos != string::npos) cmdLine = cmdLine.substr(0, pos);

                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');

                    if (pos != string::npos) cmdLine = cmdLine.substr(pos + 1);

                    // Compare against requested process name
                    if (procName == cmdLine) pid = id;
                }
            }
        }
        isRunning = pid > -1;
    }

    closedir(dp);
    return isRunning;
}
#elif defined Q_OS_WIN
auto ProcessInfo::isProcessRunningWin(const QString &name) -> bool
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (!Process32First(snapshot, &entry))
    {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (!QString::compare(QString::fromStdString(entry.szExeFile), name, Qt::CaseInsensitive))
        {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
}
#endif
