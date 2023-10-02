#pragma once

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

    [[nodiscard]] bool canQuit() const
    {
        return false;
    }
    [[nodiscard]] bool canRaise() const
    {
        return false;
    }
    [[nodiscard]] bool hasTrackList() const
    {
        return false;
    }
    [[nodiscard]] QString identity() const
    {
        return QStringLiteral("GM-Companion");
    }
    [[nodiscard]] QString desktopEntry() const
    {
        return QStringLiteral("gm-companion");
    }
    [[nodiscard]] QStringList supportedUriSchemes() const
    {
        return {};
    }
    [[nodiscard]] QStringList supportedMimeTypes() const
    {
        return {};
    }

public slots:
    Q_NOREPLY void Raise() const;
    Q_NOREPLY void Quit() const;
};
