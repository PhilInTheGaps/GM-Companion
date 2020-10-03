#ifndef AUDIOICON_H
#define AUDIOICON_H

#include <QObject>
#include <QPixmap>

class AudioIcon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imageId READ imageId WRITE setImageId NOTIFY iconChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY iconChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY iconChanged)

public:
    AudioIcon(QString image, QString title, QString subtitle, QObject *parent)
        : QObject(parent), m_imageId(image), m_title(title), m_subtitle(subtitle) {}
    AudioIcon(QString imageId, QObject *parent) : QObject(parent), m_imageId(imageId) {}
    AudioIcon(QObject *parent) : QObject(parent) {}

    QString imageId() const { return m_imageId; }
    void setImageId(QString image) { m_imageId = image; emit iconChanged(); }

    QString title() const { return m_title; }
    void setTitle(QString title) { m_title = title; emit iconChanged(); }

    QString subtitle() const { return m_subtitle; }
    void setSubtitle(QString subtitle) { m_subtitle = subtitle; emit iconChanged(); }

    QList<QPair<QString, QPixmap>> collageIcons() const { return m_collageIcons; }
    bool addCollageIcon(const QPair<QString, QPixmap> &icon);
    void setCollageIcon(const QPair<QString, QPixmap>& icon, int index);

    int lastFileIndex() const { return m_lastFileIndex; }
    void setLastFileIndex(int index) { m_lastFileIndex = index; }
    void update() { emit iconChanged(); }

signals:
    void iconChanged();

private:
    QString m_imageId, m_title, m_subtitle;
    QList<QPair<QString, QPixmap>> m_collageIcons;
    int m_lastFileIndex = 0;
};

#endif // AUDIOICON_H
