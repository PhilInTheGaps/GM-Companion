#ifndef METADATAREADER_H
#define METADATAREADER_H

#include "audiometadata.h"
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QObject>
#include <QPixmap>
#include <QTemporaryFile>

class MetaDataReader : public QObject
{
    Q_OBJECT
public:
    explicit MetaDataReader(QObject *parent = nullptr);

    AudioMetaData *metaData() const
    {
        return m_metaData;
    }
    void setMetaData(AudioMetaData *metaData);

private:
    AudioMetaData *m_metaData = nullptr;
    QTemporaryFile *m_coverFile = nullptr;

signals:
    void metaDataChanged();

public slots:
    void updateMetaData(QMediaPlayer *mediaPlayer);
    void updateMetaData(const QMediaMetaData &metaData);
    void updateMetaData(const QString &key, const QVariant &value);
    void updateMetaData(QMediaMetaData::Key key, const QVariant &value);
    void updateMetaData(const QByteArray &data);
    void updateDuration(qint64 duration);
    void clearMetaData();
};

#endif // METADATAREADER_H
