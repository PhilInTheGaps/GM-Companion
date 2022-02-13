#pragma once

#include <QString>
#include <QSize>
#include <QJsonObject>
#include <QJsonArray>

struct SpotifyImage
{
    static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyImage>;
    static auto fromJson(const QJsonArray &array) -> QVector<QSharedPointer<SpotifyImage>>;

    QString url;
    int width;
    int height;

    [[nodiscard]] auto size() const -> QSize;
};

