#pragma once

#include "src/common/projects/abstractprojectupgraderjson.h"
#include <QByteArray>
#include <QJsonArray>

class AudioProjectUpgrader : public AbstractProjectUpgraderJSON
{
public:
    explicit AudioProjectUpgrader();

    auto run() -> QByteArray override;

private:
    static auto convertCategories(const QJsonArray &categories) -> QJsonArray;
    static auto convertScenarios(const QJsonArray &scenarios) -> QJsonArray;
    static auto convertMusicElements(const QJsonArray &elements) -> QJsonArray;
    static void convertSpotifyElements(const QJsonArray &elements, QJsonArray &musicElementsNew);
    static auto convertSoundElements(const QJsonArray &elements) -> QJsonArray;
    static auto convertRadioElements(const QJsonArray &elements) -> QJsonArray;

    static constexpr int OLD_VERSION = 3;
    static constexpr int NEW_VERSION = 4;
};
