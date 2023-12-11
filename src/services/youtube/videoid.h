#pragma once

#include <QString>

namespace Services
{

class VideoId
{
public:
    VideoId() = default;
    explicit VideoId(const QString &idOrUrl);

    [[nodiscard]] auto toString() const -> QString;
    [[nodiscard]] auto toUrl() const -> QString;

    [[nodiscard]] static auto isValid(const QString &id) -> bool;
    [[nodiscard]] auto isValid() const -> bool;

private:
    [[nodiscard]] static auto normalize(const QString &idOrUrl) -> QString;

    QString m_id;
};

} // namespace Services
