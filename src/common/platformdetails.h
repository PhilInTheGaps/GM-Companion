#ifndef PLATFORMDETAILS_H
#define PLATFORMDETAILS_H

#include <QObject>

class PlatformDetails : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isAndroid READ isAndroid CONSTANT)
    Q_PROPERTY(bool isWindows READ isWindows CONSTANT)
    Q_PROPERTY(bool isMacOS   READ isMacOS   CONSTANT)

public:
    explicit PlatformDetails(QObject *parent = nullptr) : QObject(parent) {}

    static bool isAndroid();
    static bool isWindows();
    static bool isMacOS();
};

#endif // PLATFORMDETAILS_H
