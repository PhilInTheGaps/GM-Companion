#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <QObject>
#include <QJsonObject>

class AudioFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int source READ source WRITE setSource NOTIFY fileChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY fileChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY fileChanged)
    Q_PROPERTY(bool missing READ missing WRITE setMissing NOTIFY fileChanged)

public:
    AudioFile(QString url, int source, QString title, QObject *parent) :
        QObject(parent), m_source(source), m_url(url), m_title(title){}

    AudioFile(QJsonObject object, QObject *parent = nullptr) : QObject(parent)
    {
        m_url    = object["url"].toString();
        m_source = object["source"].toInt();
    }

    AudioFile(const AudioFile &other) : QObject(other.parent()) {
        m_url = other.m_url;
        m_source = other.m_source;
        m_title = other.m_title;
    }

    friend void swap(AudioFile & first, AudioFile & second) {
        // d_ptr swap doesn't take care of parentage
        QObject * firstParent = first.parent();
        QObject * secondParent = second.parent();
        first.setParent(nullptr);
        second.setParent(nullptr);
        first.d_ptr.swap(second.d_ptr);
        second.setParent(firstParent);
        first.setParent(secondParent);
    }

    AudioFile & operator=(AudioFile other) {
        swap(*this, other);
        return *this;
    }

    int source() const { return m_source; }
    void setSource(int source) { m_source = source; emit fileChanged(); }

    QString url() const { return m_url; }
    void setUrl(QString url) { m_url = url; emit fileChanged(); }

    QString title() const { return m_title; }
    void setTitle(QString title) { m_title = title; emit fileChanged(); }

    bool missing() const { return m_missing; }
    void setMissing(bool missing) { m_missing = missing; emit fileChanged(); }

    QJsonObject toJson() const
    {
        return QJsonObject {
            { "url", url() },
            { "source", source() }
        };
    }

private:
    int m_source = -1;
    QString m_url, m_title;
    bool m_missing = false;

signals:
    void fileChanged();
};

#endif // AUDIOFILE_H
