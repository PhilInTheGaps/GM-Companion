#ifndef AUDIOMETADATA_H
#define AUDIOMETADATA_H

#include <QObject>

class AudioMetaData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(QString cover READ cover WRITE setCover NOTIFY coverChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(qint64 length READ length WRITE setLength NOTIFY lengthChanged)

public:
    explicit AudioMetaData(QObject *parent = nullptr) : QObject(parent) {}

    QString title() const { return m_title; }
    void setTitle(const QString& title) { m_title = title; emit titleChanged(); }

    QString artist() const { return m_artist; }
    void setArtist(const QString& artist) { m_artist = artist; emit artistChanged(); }

    QString album() const { return m_album; }
    void setAlbum(const QString& album) { m_album = album; emit albumChanged(); }

    QString cover() const { return m_cover; }
    void setCover(const QString& cover) { m_cover = cover; emit coverChanged(); }

    QString type() const { return m_type; }
    void setType(const QString& type) { m_type = type; emit typeChanged(); }

    qint64 length() const { return m_length; }
    void setLength(qint64 length) { m_length = length; emit lengthChanged(); }

signals:
    void titleChanged();
    void artistChanged();
    void albumChanged();
    void coverChanged();
    void typeChanged();
    void lengthChanged();

private:
    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_cover;
    QString m_type;

    qint64 m_length = 0;
};

#endif // AUDIOMETADATA_H
