#ifndef AUDIOICON_H
#define AUDIOICON_H

#include <QObject>
#include <QPixmap>
#include <QUrl>
#include "audioelement.h"

class AudioIcon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imageId READ imageId WRITE setImageId NOTIFY iconChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY iconChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY iconChanged)
    Q_PROPERTY(QString absoluteUrl READ absoluteUrl NOTIFY urlChanged)
    Q_PROPERTY(QString relativeUrl READ relativeUrl WRITE setRelativeUrl NOTIFY urlChanged)

public:
    AudioIcon(const QString& image, const QString& title, const QString& subtitle, AudioElement *parent)
        : QObject(parent), element(parent), m_imageId(image), m_title(title), m_subtitle(subtitle) {}
    AudioIcon(const QString& imageId, AudioElement *parent)
        : QObject(parent), element(parent), m_imageId(imageId) {}

    QString imageId() const { return m_imageId; }
    void setImageId(const QString& image) { m_imageId = image; emit iconChanged(); }

    QString title() const { return m_title; }
    void setTitle(const QString& title) { m_title = title; emit iconChanged(); }

    QString subtitle() const { return m_subtitle; }
    void setSubtitle(const QString& subtitle) { m_subtitle = subtitle; emit iconChanged(); }

    QString absoluteUrl() const;
    QString relativeUrl() const { return m_relativeUrl; }
    void setRelativeUrl(const QString& url);

    QList<QPair<QString, QPixmap>> collageIcons() const { return m_collageIcons; }
    bool addCollageIcon(const QPair<QString, QPixmap> &icon);
    void setCollageIcon(const QPair<QString, QPixmap>& icon, int index);

    int lastFileIndex() const { return m_lastFileIndex; }
    void setLastFileIndex(int index) { m_lastFileIndex = index; }
    void update() { emit iconChanged(); }

signals:
    void iconChanged();
    void urlChanged();

private:
    AudioElement *element = nullptr;
    QString m_imageId, m_title, m_subtitle, m_relativeUrl;
    QList<QPair<QString, QPixmap>> m_collageIcons;
    int m_lastFileIndex = 0;
};

#endif // AUDIOICON_H
