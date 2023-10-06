#pragma once
#ifndef NO_DBUS

#include "../metadata/metadatareader.h"
#include "mprisadaptor.h"
#include "mprisplayeradaptor.h"
#include <QObject>
#include <memory>

class MprisManager : public QObject
{
    Q_OBJECT
public:
    explicit MprisManager(QObject *parent = nullptr);

    void setPlaybackStatus(AudioPlayer::State status);
    void setVolume(double volume);
    void setMetaDataReader(MetaDataReader *reader);

signals:
    void play();
    void playPause();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(double volume);

private:
    void sendMprisUpdateSignal(const QString &property, const QVariant &value) const;
    void sendUpdatedMetadata();

    QHash<QString, QVariant> m_metaData;

    // must be created on the heap using _new_
    MprisAdaptor *m_mprisAdaptor = nullptr;
    MprisPlayerAdaptor *m_mprisPlayerAdaptor = nullptr;
};

#endif
