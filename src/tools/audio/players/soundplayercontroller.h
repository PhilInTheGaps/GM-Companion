#pragma once

#include "audioplayer.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QNetworkAccessManager>

class SoundPlayer;
class AudioElement;

class SoundPlayerController : public AudioPlayer
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    READ_LIST_PROPERTY(AudioElement, activeElements)

public:
    explicit SoundPlayerController(QNetworkAccessManager &networkManager, QObject *parent = nullptr);

    void play(AudioElement *elements);
    void stop(const QString &element);

public slots:
    void play() override
    { /* Not Implemented */
    }
    void pause() override
    { /* Not Implemented */
    }
    void stop() override
    {
        emit stopAll();
    }
    void setVolume(int linear, int logarithmic) override;
    void next() override
    { /* Not Implemented */
    }
    void again() override
    { /* Not Implemented */
    }

private:
    QNetworkAccessManager &m_networkManager;
    QList<SoundPlayer *> m_players;
    int m_linearVolume = 0;
    int m_logarithmicVolume = 0;

    [[nodiscard]] auto elements() const -> QList<AudioElement *>;
    [[nodiscard]] auto isSoundPlaying(AudioElement *elements) const -> bool;
    void updateActiveElements();

private slots:
    void onPlayerStateChanged(State state);
    void onSoundsChanged(const QList<AudioElement *> &sounds);

signals:
    void setPlayerVolume(int linear, int logarithmic);
    void stopElement(QString element);
    void stopAll();
    void soundsChanged(QList<AudioElement *>);
};
