#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QPixmap>
#include <QUrl>
#include <QtQml/qqmlregistration.h>

class AudioElement;

class AudioThumbnail : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString imageId READ imageIdForReload WRITE setImageId NOTIFY thumbnailChanged)
    Q_PROPERTY(QString absoluteUrl READ absoluteUrl NOTIFY urlChanged)
    Q_PROPERTY(QString relativeUrl READ relativeUrl WRITE setRelativeUrl NOTIFY urlChanged)
    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY(QString, subtitle)
    AUTO_PROPERTY_VAL2(int, lastFileIndex, 0)

public:
    explicit AudioThumbnail(const QString &imageId, AudioElement *parent);

    [[nodiscard]] auto imageId() const -> QString
    {
        return m_imageId;
    }
    [[nodiscard]] auto imageIdForReload() -> QString;
    void setImageId(const QString &image)
    {
        m_imageId = image;
        emit thumbnailChanged();
    }

    [[nodiscard]] auto absoluteUrl() const -> QString;
    [[nodiscard]] auto relativeUrl() const -> QString
    {
        return m_relativeUrl;
    }
    void setRelativeUrl(const QString &url);

    [[nodiscard]] auto collageImages() const -> QList<std::pair<QString, QPixmap>>
    {
        return m_collageIcons;
    }
    auto addCollageImage(const std::pair<QString, QPixmap> &icon) -> bool;

    void update()
    {
        emit thumbnailChanged();
    }

signals:
    void thumbnailChanged();
    void urlChanged();

private:
    AudioElement *element = nullptr;
    QString m_imageId;
    QString m_relativeUrl;
    QList<std::pair<QString, QPixmap>> m_collageIcons;
    bool m_imageIdCounter = false;
};

Q_DECLARE_METATYPE(AudioThumbnail)
