#pragma once

#include "bufferedaudioplayer.h"
#include <QtQml/qqmlregistration.h>

class AudioElement;
class AudioFile;

class SoundPlayer : public BufferedAudioPlayer
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit SoundPlayer(QNetworkAccessManager &networkManager, QObject *parent = nullptr);

public slots:
    void stopElement(const QString &name);
};
