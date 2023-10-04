#pragma once

#include "audioplayer.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class SoundPlayer;
class AudioElement;

class SoundPlayerController : public AudioPlayer
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    READ_LIST_PROPERTY(AudioElement, activeElements)

public:
    explicit SoundPlayerController(QObject *parent = nullptr);

    void play(AudioElement *elements);
    void stop(const QString &element);

public slots:
    void play() override
    {
    }
    void pause() override
    {
    }
    void stop() override
    {
        emit stopAll();
    }
    void setVolume(int linear, int logarithmic) override;
    void next() override
    {
    }
    void again() override
    {
    }

private:
    QList<SoundPlayer *> m_players;
    int m_volume = 0;

    [[nodiscard]] auto elements() const -> QList<AudioElement *>;
    [[nodiscard]] auto isSoundPlaying(AudioElement *elements) const -> bool;
    void updateActiveElements();

private slots:
    void onPlayerStopped(SoundPlayer *player);
    void onSoundsChanged(const QList<AudioElement *> &sounds);

signals:
    void setPlayerVolume(int linear, int logarithmic);
    void stopElement(QString element);
    void stopAll();
    void soundsChanged(QList<AudioElement *>);
};
