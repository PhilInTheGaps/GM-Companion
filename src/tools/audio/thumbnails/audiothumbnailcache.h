#pragma once

#include <QHash>
#include <QObject>
#include <QPixmap>
#include <QPointer>
#include <QReadWriteLock>

class AudioThumbnailCache : public QObject
{
    Q_OBJECT
public:
    static auto instance() -> AudioThumbnailCache *;

    static auto tryGet(const QString &key, QPixmap *pixmap) -> bool;
    static auto contains(const QString &key) -> bool;

public slots:
    void insertImage(const QString &key, const QPixmap &image);

private:
    using QObject::QObject;

    static inline QPointer<AudioThumbnailCache> m_instance = nullptr;

    static constexpr int LOCK_TIMEOUT = 250;

    QHash<QString, QPixmap> m_cache;
    QReadWriteLock lock;
};
