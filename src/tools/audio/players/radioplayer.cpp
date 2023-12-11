#include "radioplayer.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioRadio, "gm.audio.radio")

RadioPlayer::RadioPlayer(QNetworkAccessManager *networkManager, MetaDataReader &metaDataReader, QObject *parent)
    : BufferedAudioPlayer(u"radio"_s, networkManager, parent), m_metaDataReader(metaDataReader)
{
    connect(this, &RadioPlayer::metaDataChanged, this, &RadioPlayer::onMetaDataChanged);
}

void RadioPlayer::onMetaDataChanged(const QMediaMetaData &data)
{
    m_metaDataReader.setMetaData(data);
}
