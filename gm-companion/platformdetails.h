#ifndef PLATFORMDETAILS_H
#define PLATFORMDETAILS_H

#include <QObject>

class PlatformDetails : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isAndroid READ isAndroid NOTIFY isAndroidChanged)

public:
    explicit PlatformDetails(QObject *parent = nullptr);

    bool isAndroid();

signals:
    void isAndroidChanged();

public slots:
};

#endif // PLATFORMDETAILS_H
