#include "processinfo.h"
#include <QtGlobal>
#include <QString>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#ifdef Q_OS_WIN
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#endif

using namespace std;

bool ProcessInfo::isProcessRunning(std::string procName)
{
    bool isRunning = false;

    #ifdef Q_OS_LINUX
    // https://proswdev.blogspot.com/2012/02/get-process-id-by-name-in-linux-using-c.html
    // Open the /proc directory
    DIR *dp = opendir("/proc");

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

    #elif defined Q_OS_WIN
    // https://stackoverflow.com/a/57164620

    PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (!Process32First(snapshot, &entry))
        {
            CloseHandle(snapshot);
            return false;
        }

        do {
            if (!QString::compare(QString::fromStdString(entry.szExeFile), QString::fromStdString(procName), Qt::CaseInsensitive))
            {
                CloseHandle(snapshot);
                isRunning = true;
            }
        } while (Process32Next(snapshot, &entry));

        CloseHandle(snapshot);
        isRunning = false;

    #endif

    return isRunning;
}
