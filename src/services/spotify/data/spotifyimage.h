#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QSize>
#include <QString>
#include <vector>

namespace Services
{

struct SpotifyImage
{
    static auto fromJson(const QJsonObject &json) -> SpotifyImage;
    static auto fromJson(const QJsonArray &array) -> std::vector<SpotifyImage>;

    QString url;
    int width;
    int height;

    [[nodiscard]] auto size() const -> QSize;
};

} // namespace Services
