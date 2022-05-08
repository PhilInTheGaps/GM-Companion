#include "processinfo.h"
#include <QtGlobal>
#include <QLoggingCategory>

#include <sys/types.h>
#include <dirent.h>
#include <cerrno>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <QDir>

Q_LOGGING_CATEGORY(gmProcessInfo, "gm.utils.processinfo")

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

auto ProcessInfo::isProcessRunning(const QString &procName) -> bool
{
    #ifdef Q_OS_MACOS

    return isProcessRunningMac(procName.toUtf8());

    #elif defined Q_OS_UNIX

    return isProcessRunningUnix(procName);

    #elif defined Q_OS_WIN
    // https://stackoverflow.com/a/57164620

    for (const auto &name : { procName, QString("%1%2").arg(procName, ".exe") })
    {
        if (isProcessRunningWin(name)) return true;
    }
    return false;

    #else

    qCWarning(gmProcessInfo) << "Error: Method isProcessRunning() is not defined for this operating system!";

    return false;
    #endif
}

#ifdef Q_OS_MACOS
auto ProcessInfo::isProcessRunningMac(const QString &procName) -> bool
{
    // https://stackoverflow.com/questions/49506579/how-to-find-the-pid-of-any-process-in-mac-osx-c

    pid_t pids[2048];
    int bytes = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    int n_proc = bytes / sizeof(pids[0]);
    const auto procNameC = procName.toUtf8().constData();

    for (int i = 0; i < n_proc; i++)
    {
        struct proc_bsdinfo proc;
        int st = proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0, &proc, PROC_PIDTBSDINFO_SIZE);

        if (st == PROC_PIDTBSDINFO_SIZE)
        {
            if (strcmp(procNameC, proc.pbi_name) == 0)
            {
                return true;
            }
        }
    }

    return false;
}
#elif defined Q_OS_UNIX
#include <QDebug>
// Based on https://proswdev.blogspot.com/2012/02/get-process-id-by-name-in-linux-using-c.html
// but implemented using Qt classes
auto ProcessInfo::isProcessRunningUnix(const QString &procName) -> bool
{
    QDir procDir(QStringLiteral("/proc"));
    auto entries = procDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Readable);

    bool isNumeric = false;

    for (const auto& entry : entries)
    {
        entry.toInt(&isNumeric);

        if (!isNumeric) continue;

        // Read contents of virtual /proc/{pid}/cmdline file
        auto cmdPath = QStringLiteral("/proc/%1/cmdline").arg(entry);
        auto cmdFile = QFile(cmdPath);

        if (cmdFile.open(QIODevice::ReadOnly))
        {
            auto content = cmdFile.readAll();
            cmdFile.close();

            if (!content.isEmpty())
            {
                // Get program path
                content = content.left(content.indexOf('\0'));

                // Keep program name only, removing the path
                content = content.right(content.length() - content.lastIndexOf('/') - 1);

                if (procName == content) return true;
            }
        }
    }

    return false;
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
