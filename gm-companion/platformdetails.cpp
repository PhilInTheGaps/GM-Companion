#include "platformdetails.h"

PlatformDetails::PlatformDetails(QObject *parent) : QObject(parent)
{
}

bool PlatformDetails::isAndroid()
{
#ifdef Q_OS_ANDROID
    return true;

#else // ifdef Q_OS_ANDROID
    return false;

#endif // ifdef Q_OS_ANDROID
}
