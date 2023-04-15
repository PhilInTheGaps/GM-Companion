#pragma once

#include <QQuickImageProvider>
#include <QRegularExpression>

class AudioThumbnailProvider : public QQuickImageProvider
{
    Q_DISABLE_COPY_MOVE(AudioThumbnailProvider)
public:
    AudioThumbnailProvider();
    ~AudioThumbnailProvider() override = default;

    auto requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) -> QPixmap override;

private:
    static constexpr const char *IMAGE_ID_REGEX = R"((.+\/(.+)\/.+)(\?r=))";
    inline static const auto regex = QRegularExpression(IMAGE_ID_REGEX);
};
