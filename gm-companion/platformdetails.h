#ifndef PLATFORMDETAILS_H
#define PLATFORMDETAILS_H

#include <QObject>

class PlatformDetails : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isAndroid READ isAndroid NOTIFY isAndroidChanged)
    Q_PROPERTY(bool isWindows READ isWindows NOTIFY isWindowsChanged)
    Q_PROPERTY(bool isMacOS READ isMacOS NOTIFY isMacOSChanged)

public:
    explicit PlatformDetails(QObject *parent = nullptr);

    bool isAndroid();
    bool isWindows();
    bool isMacOS();

signals:
    void isAndroidChanged();
    void isWindowsChanged();
    void isMacOSChanged();

};

#endif // PLATFORMDETAILS_H
