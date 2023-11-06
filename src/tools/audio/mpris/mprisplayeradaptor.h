#pragma once
#ifndef NO_DBUS

#include "../players/audioplayer.h"
#include <QDBusAbstractAdaptor>
#include <QDBusArgument>
#include <QDBusObjectPath>
#include <QObject>
#include <QTemporaryFile>

class MprisPlayerAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
    Q_PROPERTY(QString PlaybackStatus READ playbackStatus NOTIFY playbackStatusChanged)
    Q_PROPERTY(QString LoopStatus READ loopStatus WRITE setLoopStatus)
    Q_PROPERTY(double Rate READ rate WRITE setRate)
    Q_PROPERTY(bool Shuffle READ shuffle WRITE setShuffle)
    Q_PROPERTY(QHash<QString, QVariant> Metadata READ metadata NOTIFY metadataChanged)
    Q_PROPERTY(double Volume READ volume WRITE setVolume)
    Q_PROPERTY(qlonglong Position READ position)
    Q_PROPERTY(double MinimumRate READ minimumRate)
    Q_PROPERTY(double MaximumRate READ maximumRate)
    Q_PROPERTY(bool CanGoNext READ canGoNext)
    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious)
    Q_PROPERTY(bool CanPlay READ canPlay)
    Q_PROPERTY(bool CanPause READ canPause)
    Q_PROPERTY(bool CanSeek READ canSeek)
    Q_PROPERTY(bool CanControl READ canControl CONSTANT)

public:
    explicit MprisPlayerAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent){};

    [[nodiscard]] auto playbackStatus() const -> QString
    {
        return m_PlaybackStatus;
    }
    void setPlaybackStatus(AudioPlayer::State status);

    [[nodiscard]] auto loopStatus() const -> QString
    {
        return m_LoopStatus;
    }
    void setLoopStatus(const QString & /*status*/) const
    { /* Not Implemented */
    }

    [[nodiscard]] auto rate() const -> double
    {
        return 1.0;
    }
    void setRate(double /*rate*/) const
    { /* Not Implemented */
    }

    [[nodiscard]] auto shuffle() const -> bool
    {
        return m_Shuffle;
    }
    void setShuffle(bool /*set*/) const
    { /* Not Implemented */
    }

    [[nodiscard]] auto metadata() const -> QHash<QString, QVariant>
    {
        return m_Metadata;
    }
    void setMetadata(const QHash<QString, QVariant> &data);

    [[nodiscard]] auto volume() const -> double
    {
        return m_Volume;
    }
    void setVolume(double /*volume*/) const
    { /* Not Implemented */
    }

    [[nodiscard]] auto position() const -> qlonglong
    {
        return m_Position;
    }
    [[nodiscard]] auto minimumRate() const -> double
    {
        return 1.0;
    }
    [[nodiscard]] auto maximumRate() const -> double
    {
        return 1.0;
    }

    [[nodiscard]] auto canGoNext() const -> bool
    {
        return true;
    }
    [[nodiscard]] auto canGoPrevious() const -> bool
    {
        return true;
    }
    [[nodiscard]] auto canPlay() const -> bool
    {
        return true;
    }
    [[nodiscard]] auto canPause() const -> bool
    {
        return true;
    }
    [[nodiscard]] auto canSeek() const -> bool
    {
        return false;
    }
    [[nodiscard]] auto canControl() const -> bool
    {
        return true;
    }

private:
    QString m_PlaybackStatus = QStringLiteral("Stopped");
    QString m_LoopStatus;
    double m_Volume = 0;
    bool m_Shuffle = false;
    QHash<QString, QVariant> m_Metadata;
    qlonglong m_Position = 0;
    QTemporaryFile m_tempArtFile;

signals:
    void Seeked(qlonglong Position);
    void next();
    void changeVolume(double volume);
    void previous();
    void pause();
    void playPause();
    void stop();
    void play();

    void playbackStatusChanged(QString);
    void metadataChanged(QHash<QString, QVariant>);

public slots:
    Q_NOREPLY void Next()
    {
        emit next();
    }
    Q_NOREPLY void Previous()
    {
        emit previous();
    }
    Q_NOREPLY void Pause()
    {
        emit pause();
    }
    Q_NOREPLY void PlayPause()
    {
        emit playPause();
    }
    Q_NOREPLY void Stop()
    {
        emit stop();
    }
    Q_NOREPLY void Play()
    {
        emit play();
    }

    // Not actually implemented
    Q_NOREPLY void Seek(qlonglong) const;
    Q_NOREPLY void SetPosition(const QDBusObjectPath &, qlonglong) const;
    Q_NOREPLY void OpenUri(const QString &) const;
};

#endif
