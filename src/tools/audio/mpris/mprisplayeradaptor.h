#ifndef MPRISPLAYERADAPTOR_H
#define MPRISPLAYERADAPTOR_H

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QDBusObjectPath>
#include <QDBusArgument>

class MprisPlayerAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
    Q_PROPERTY(QString PlaybackStatus READ playbackStatus NOTIFY playbackStatusChanged)
    Q_PROPERTY(QString LoopStatus READ loopStatus WRITE setLoopStatus)
    Q_PROPERTY(double Rate READ rate WRITE setRate)
    Q_PROPERTY(bool Shuffle READ shuffle WRITE setShuffle)
    Q_PROPERTY(QMap Metadata READ metadata NOTIFY metadataChanged)
    Q_PROPERTY(double Volume READ volume WRITE setVolume)
    Q_PROPERTY(qlonglong Position READ position)
    Q_PROPERTY(double MinimumRate READ minimumRate)
    Q_PROPERTY(double MaximumRate READ maximumRate)
    Q_PROPERTY(bool CanGoNext READ canGoNext)
    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious)
    Q_PROPERTY(bool CanPlay READ canPlay)
    Q_PROPERTY(bool CanPause READ canPause)
    Q_PROPERTY(bool CanSeek READ canSeek)
    Q_PROPERTY(bool CanControl READ canControl)

public:
    MprisPlayerAdaptor(QObject *obj) : QDBusAbstractAdaptor(obj) {}

    QString playbackStatus() const { return m_PlaybackStatus; }
    void setPlaybackStatus(int status);

    QString loopStatus() const { return m_LoopStatus; }
    void setLoopStatus(QString status) { /* Not Implemented */ }

    double rate() const { return 1.0; }
    void setRate(double rate) { /* Not Implemented */ }

    bool shuffle() const { return m_Shuffle; }
    void setShuffle(bool set) { /* Not Implemented */ }

    QMap<QString, QVariant> metadata() const { return m_Metadata; }
    void setMetadata(QMap<QString, QVariant> data) { m_Metadata = data; emit metadataChanged(data); }

    double volume() const { return m_Volume; }
    void setVolume(double volume) {  } // emit changeVolume(volume);

    qlonglong position() const { return m_Position; }
    double minimumRate() const { return 1.0; }
    double maximumRate() const { return 1.0; }

    bool canGoNext() const { return true; }
    bool canGoPrevious() const { return true; }
    bool canPlay() const { return true; }
    bool canPause() const { return true; }
    bool canSeek() const { return false; }
    bool canControl() const { return true; }

private:
    QString m_PlaybackStatus = "Stopped", m_LoopStatus;
    double m_Volume;
    bool m_Shuffle;
    QMap<QString, QVariant> m_Metadata;
    qlonglong m_Position;

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
    void metadataChanged(QMap<QString, QVariant>);

public slots:
    Q_NOREPLY void Next() { emit next(); }
    Q_NOREPLY void Previous() { emit previous(); }
    Q_NOREPLY void Pause() { emit pause(); }
    Q_NOREPLY void PlayPause() { emit playPause();}
    Q_NOREPLY void Stop() { emit stop(); }
    Q_NOREPLY void Play() { emit play(); }

    // Not actually implemented
    Q_NOREPLY void Seek(qlonglong Offset);
    Q_NOREPLY void SetPosition(QDBusObjectPath TrackId, qlonglong Position);
    Q_NOREPLY void OpenUri(QString Uri);
};

#endif // MPRISPLAYERADAPTOR_H
