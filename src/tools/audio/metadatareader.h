#ifndef METADATAREADER_H
#define METADATAREADER_H

#include <QObject>
#include <QMediaPlayer>
#include <QPixmap>
#include <QTemporaryFile>

class MetaData
{
public:
    MetaData() {}
    ~MetaData() {}

    QString artist = "", album = "", title = "", cover = "", coverUrl = "", type = "", elementIcon = "";
    qint64 length = 0;
};

class MetaDataReader : public QObject
{
    Q_OBJECT
public:
    explicit MetaDataReader(QObject *parent = nullptr);
    void setMetaData(MetaData m);

private:
    MetaData m_metaData;
    QTemporaryFile m_coverFile;

signals:
    void metaDataUpdated(const MetaData& m);

public slots:
    void updateMetaData(QMediaPlayer *mediaPlayer);
    void updateMetaData(const QString& key, const QVariant &value);
    void updateMetaData(const QByteArray& data);
    void updateDuration(const qint64& duration);
    void clearMetaData();
};

#endif // METADATAREADER_H
