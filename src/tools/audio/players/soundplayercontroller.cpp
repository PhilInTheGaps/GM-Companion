#include "soundplayercontroller.h"
#include "../project/audioelement.h"
#include "soundplayer.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmAudioSoundController, "gm.audio.sounds.controller")

SoundPlayerController::SoundPlayerController(QNetworkAccessManager &networkManager, QObject *parent)
    : AudioPlayer(parent), m_networkManager(networkManager)
{
    connect(this, &SoundPlayerController::soundsChanged, this, &SoundPlayerController::onSoundsChanged);
}

/**
 * @brief Play a sound element
 * @param element SoundElement to be played
 */
void SoundPlayerController::play(AudioElement *element)
{
    if (!element) return;

    qCDebug(gmAudioSoundController()) << "Playing sound list:" << element->name() << "...";

    if (!isSoundPlaying(element))
    {
        auto *player = new SoundPlayer(m_networkManager, this);
        player->setVolume(m_linearVolume, m_logarithmicVolume);

        connect(player, &SoundPlayer::stateChanged, this, &SoundPlayerController::onPlayerStateChanged);
        connect(this, &SoundPlayerController::setPlayerVolume, player, &SoundPlayer::setVolume);
        connect(this, &SoundPlayerController::stopAll, player, &SoundPlayer::stop);
        connect(this, &SoundPlayerController::stopElement, player, &SoundPlayer::stopElement);

        m_players.append(player);
        player->play(element);

        emit soundsChanged(elements());
    }
}

void SoundPlayerController::stop(const QString &element)
{
    emit stopElement(element);
}

/**
 * @brief Check if a sound element is currently playing
 * @param element SoundElement to be checked
 * @return True if element is playing
 */
auto SoundPlayerController::isSoundPlaying(AudioElement *element) const -> bool
{
    qCDebug(gmAudioSoundController()) << "Checking if sound" << element->name() << "is playing ...";

    foreach (const auto *player, m_players)
    {
        if (player && (player->element() == element))
        {
            return true;
        }
    }

    return false;
}

void SoundPlayerController::updateActiveElements()
{
    a_activeElements.clear();
    a_activeElements.reserve(m_players.size());

    foreach (const auto *player, m_players)
    {
        if (player)
        {
            a_activeElements.append(player->element());
        }
    }

    emit activeElementsChanged();
}

/**
 * @brief Set the volume for all active sound elements
 * @param volume Volume value
 */
void SoundPlayerController::setVolume(int linear, int logarithmic)
{
    m_linearVolume = linear;
    m_logarithmicVolume = logarithmic;
    emit setPlayerVolume(linear, logarithmic);
}

/**
 * @brief Get all active sound elements
 */
auto SoundPlayerController::elements() const -> QList<AudioElement *>
{
    QList<AudioElement *> elements;

    foreach (const auto *player, m_players)
    {
        if (player) elements.append(player->element());
    }

    return elements;
}

void SoundPlayerController::onPlayerStateChanged(State state)
{
    // A sound player stopped. Remove it from list and delete it.
    if (state == State::Stopped)
    {
        auto *player = qobject_cast<SoundPlayer *>(sender());
        if (!player) return;

        m_players.removeOne(player);
        player->deleteLater();

        emit soundsChanged(elements());
    }
}

void SoundPlayerController::onSoundsChanged(const QList<AudioElement *> &sounds)
{
    Q_UNUSED(sounds)
    updateActiveElements();
}
