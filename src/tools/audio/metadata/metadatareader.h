#pragma once

#include "audiometadata.h"
#include <QByteArray>
#include <QMediaMetaData>
#include <QTemporaryFile>
#include <QVariant>
#include <memory>

class MetaDataReader
{
public:
    MetaDataReader() = default;

    [[nodiscard]] auto metaData() -> AudioMetaData *
    {
        return &m_metaData;
    }
    void setMetaData(const AudioMetaData &metaData);

    void setMetaData(const QString &key, const QVariant &value);
    void setMetaData(QMediaMetaData::Key key, const QVariant &value);
    void setMetaData(const QMediaMetaData &data);
    void setDuration(qint64 duration);

    void loadMetaData(const QString &path, const QByteArray &data);

    void clearMetaData();

private:
    AudioMetaData m_metaData;
    std::unique_ptr<QTemporaryFile> m_coverFile = nullptr;
};
