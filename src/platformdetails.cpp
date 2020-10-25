#include "platformdetails.h"

bool PlatformDetails::isAndroid()
{
#ifdef Q_OS_ANDROID
    return true;

#else // ifdef Q_OS_ANDROID
    return false;

#endif // ifdef Q_OS_ANDROID
}

bool PlatformDetails::isWindows()
{
#ifdef Q_OS_WIN
    return true;

#else // ifdef Q_OS_WIN
    return false;

#endif // ifdef Q_OS_WIN
}

bool PlatformDetails::isMacOS()
{
#ifdef Q_OS_MAC
    return true;

#else // ifdef Q_OS_MAC
    return false;

#endif // ifdef Q_OS_MAC
}
