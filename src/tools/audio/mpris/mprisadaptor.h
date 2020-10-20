#ifndef MPRISADAPTOR_H
#define MPRISADAPTOR_H

#include <QObject>
#include <QDBusAbstractAdaptor>

class MprisAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
    Q_PROPERTY(bool CanQuit READ canQuit CONSTANT)
    Q_PROPERTY(bool CanRaise READ canRaise CONSTANT)
    Q_PROPERTY(bool HasTrackList READ hasTrackList CONSTANT)
    Q_PROPERTY(QString Identity READ identity CONSTANT)
    Q_PROPERTY(QString DesktopEntry READ desktopEntry CONSTANT)
    Q_PROPERTY(QStringList SupportedUriSchemes READ supportedUriSchemes CONSTANT)
    Q_PROPERTY(QStringList SupportedMimeTypes READ supportedMimeTypes CONSTANT)

public:
    MprisAdaptor(QObject *obj) : QDBusAbstractAdaptor(obj) {}

    bool canQuit() const { return false; }
    bool canRaise() const { return false; }
    bool hasTrackList() const { return false; }
    QString identity() const { return "GM-Companion"; }
    QString desktopEntry() const { return "gm-companion"; }
    QStringList supportedUriSchemes() const { return {}; }
    QStringList supportedMimeTypes() const { return {}; }

public slots:
    Q_NOREPLY void Raise();
    Q_NOREPLY void Quit();
};

#endif // MPRISADAPTOR_H
