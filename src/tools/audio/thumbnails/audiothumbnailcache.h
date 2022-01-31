#pragma once

#include <QObject>
#include <QPointer>
#include <QPixmap>
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
    explicit AudioThumbnailCache(QObject *parent = nullptr) : QObject(parent) {}
    static inline QPointer<AudioThumbnailCache> m_instance = nullptr;

    static constexpr int LOCK_TIMEOUT = 250;

    QHash<QString, QPixmap> m_cache;
    QReadWriteLock lock;

};
