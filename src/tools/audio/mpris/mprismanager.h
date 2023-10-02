#pragma once

#include "../metadata/metadatareader.h"
#include "mprisadaptor.h"
#include "mprisplayeradaptor.h"
#include <QObject>

class MprisManager : public QObject
{
    Q_OBJECT
public:
    explicit MprisManager(QObject *parent = nullptr);

    void setPlaybackStatus(int status);
    void setVolume(double volume);
    void updateMetaData(AudioMetaData *metaData);

private:
    MprisAdaptor *mprisAdaptor = nullptr;
    MprisPlayerAdaptor *mprisPlayerAdaptor = nullptr;

    void sendMprisUpdateSignal(const QString &property, const QVariant &value) const;

signals:
    void play();
    void playPause();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(double volume);
};
