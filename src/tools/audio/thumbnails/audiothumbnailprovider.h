#pragma once

#include <QQuickImageProvider>
#include <QRegularExpression>

class AudioThumbnailProvider : public QQuickImageProvider
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(AudioThumbnailProvider)
public:
    explicit AudioThumbnailProvider();
    ~AudioThumbnailProvider() override = default;

    auto requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) -> QPixmap override;

private:
    static constexpr const char *IMAGE_ID_REGEX = R"((.+\/(.+)\/.+)(\?r=))";
    inline static const auto regex = QRegularExpression(IMAGE_ID_REGEX);
};
