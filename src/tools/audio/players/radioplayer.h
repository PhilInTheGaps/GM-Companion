#pragma once

#include "../metadata/metadatareader.h"
#include "bufferedaudioplayer.h"
#include <QNetworkAccessManager>

class RadioPlayer : public BufferedAudioPlayer
{
    Q_OBJECT
public:
    explicit RadioPlayer(QNetworkAccessManager &networkManager, MetaDataReader &metaDataReader,
                         QObject *parent = nullptr);

signals:
    void metaDataChanged(const QMediaMetaData &metaData);

private slots:
    void onMetaDataChanged(const QMediaMetaData &data);

private:
    MetaDataReader &m_metaDataReader;
};
