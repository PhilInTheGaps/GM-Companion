#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QSize>
#include <QString>

struct SpotifyImage
{
    static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyImage>;
    static auto fromJson(const QJsonArray &array) -> QList<QSharedPointer<SpotifyImage>>;

    QString url;
    int width;
    int height;

    [[nodiscard]] auto size() const -> QSize;
};
