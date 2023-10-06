#pragma once
#ifndef NO_DBUS

#include <QDBusAbstractAdaptor>
#include <QObject>

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
    explicit MprisAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent){};

    [[nodiscard]] auto canQuit() const -> bool
    {
        return false;
    }
    [[nodiscard]] auto canRaise() const -> bool
    {
        return false;
    }
    [[nodiscard]] auto hasTrackList() const -> bool
    {
        return false;
    }
    [[nodiscard]] auto identity() const -> QString
    {
        return QStringLiteral("GM-Companion");
    }
    [[nodiscard]] auto desktopEntry() const -> QString
    {
        return QStringLiteral("gm-companion");
    }
    [[nodiscard]] auto supportedUriSchemes() const -> QStringList
    {
        return {};
    }
    [[nodiscard]] auto supportedMimeTypes() const -> QStringList
    {
        return {};
    }

public slots:
    Q_NOREPLY void Raise() const;
    Q_NOREPLY void Quit() const;
};

#endif
