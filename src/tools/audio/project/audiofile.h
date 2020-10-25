#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <QObject>
#include <QJsonObject>

class AudioFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY fileChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY fileChanged)
    Q_PROPERTY(QString printableUrl READ printableUrl NOTIFY fileChanged)
    Q_PROPERTY(int source READ source WRITE setSource NOTIFY fileChanged)
    Q_PROPERTY(bool missing READ missing WRITE setMissing NOTIFY fileChanged)

public:
    AudioFile(const QString &url, int source, const QString &title, QObject *parent) :
        QObject(parent), m_source(source), m_url(url), m_title(title){}

    AudioFile(const QJsonObject &object, QObject *parent = nullptr) : QObject(parent)
    {
        m_url    = object["url"].toString();
        m_source = object["source"].toInt();
    }

    /// 0: file, 1: url, 2: spotify, 3: youtube
    int source() const { return m_source; }
    void setSource(int source) { m_source = source; emit fileChanged(); }

    QString url() const { return m_url; }
    void setUrl(QString url) { m_url = url; emit fileChanged(); }

    QString title() const { return m_title; }
    void setTitle(QString title) { m_title = title; emit fileChanged(); }

    QString printableUrl() const { return title().isEmpty() ? url() : QString("%1 | %2").arg(title(), url()); }

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
