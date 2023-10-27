#pragma once

#include "../project/audioelement.h"
#include "../project/audioscenario.h"
#include <QNetworkAccessManager>
#include <QPixmap>
#include <QPointer>

class AudioThumbnailGenerator
{
public:
    static void generateThumbnails(AudioScenario *scenario);

    static auto getPlaceholderImage(AudioElement *element) -> QPixmap;
    static auto getPlaceholderImage(const QString &type) -> QPixmap;
    static auto getPlaceholderImage(AudioElement::Type type) -> QPixmap;

protected:
    static void generateThumbnail(AudioElement *element);

private:
    static auto emptyPixmap() -> QPixmap;
    static QPointer<QNetworkAccessManager> networkManager;
    static void configureNetworkManager();
    static void receivedImage(AudioElement *element, const QPixmap &pixmap, bool makeFallbackCollage);
};
