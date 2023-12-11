#include "soundplayer.h"
#include "../project/audioelement.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioSounds, "gm.audio.sounds")

SoundPlayer::SoundPlayer(QNetworkAccessManager *networkManager, QObject *parent)
    : BufferedAudioPlayer(u"sounds"_s, networkManager, parent)
{
}

void SoundPlayer::stopElement(const QString &name)
{
    if (element() && (element()->name() == name))
    {
        stop();
    }
}
