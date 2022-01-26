#ifndef AUDIOICON_H
#define AUDIOICON_H

#include <QObject>
#include <QPixmap>
#include <QUrl>
#include "audioelement.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AudioIcon : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString imageId READ imageIdForReload WRITE setImageId NOTIFY imageIdChanged)
    Q_PROPERTY(QString absoluteUrl READ absoluteUrl NOTIFY relativeUrlChanged)

    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY(QString, subtitle)
    AUTO_PROPERTY(QString, relativeUrl)

public:
    AudioIcon(const QString& image, const QString& title, const QString& subtitle, AudioElement *parent)
        : QObject(parent), element(parent), m_imageId(image), a_title(title), a_subtitle(subtitle) {}
    AudioIcon(const QString& imageId, AudioElement *parent)
        : QObject(parent), element(parent), m_imageId(imageId) {}

    QString imageId() const { return m_imageId; }
    QString imageIdForReload();
    void setImageId(const QString &id) { m_imageId = id; emit imageIdChanged(); }

    QString absoluteUrl() const;

    QList<QPair<QString, QPixmap>> collageIcons() const { return m_collageIcons; }
    bool addCollageIcon(const QPair<QString, QPixmap> &icon);
    void setCollageIcon(const QPair<QString, QPixmap>& icon, int index);

    int lastFileIndex() const { return m_lastFileIndex; }
    void setLastFileIndex(int index) { m_lastFileIndex = index; }
    void update() { emit imageIdChanged(); }

signals:
    void imageIdChanged();

private:
    AudioElement *element = nullptr;
    QString m_imageId;
    QList<QPair<QString, QPixmap>> m_collageIcons;
    int m_lastFileIndex = 0;
    bool m_imageIdCounter = false;
};

#endif // AUDIOICON_H
