#include "audioprojectupgrader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioUpgrader, "gm.audio.project.upgrader")

AudioProjectUpgrader::AudioProjectUpgrader() : AbstractProjectUpgraderJSON(OLD_VERSION)
{
}

auto AudioProjectUpgrader::run() -> QByteArray
{
    if (!isProjectCompatible()) return {};

    qCDebug(gmAudioUpgrader()) << "Converting project to newest version ...";

    const auto project = json().object();
    const auto categories = project[QStringLiteral("categories")].toArray();
    const auto categoriesNew = convertCategories(categories);

    const QJsonObject projectNew{{"name", project["name"]}, {"version", NEW_VERSION}, {"categories", categoriesNew}};
    return QJsonDocument(projectNew).toJson();
}

auto AudioProjectUpgrader::convertCategories(const QJsonArray &categories) -> QJsonArray
{
    QJsonArray categoriesNew;

    for (const auto &category : categories)
    {
        QJsonObject categoryNew{{"name", category.toObject()["name"]}};
        const auto scenarios = category.toObject()[QStringLiteral("scenarios")].toArray();

        categoryNew.insert(QStringLiteral("scenarios"), convertScenarios(scenarios));
        categoriesNew.append(categoryNew);
    }

    return categoriesNew;
}

auto AudioProjectUpgrader::convertScenarios(const QJsonArray &scenarios) -> QJsonArray
{
    QJsonArray scenariosNew;

    foreach (const auto &scenario, scenarios)
    {
        QJsonObject scenarioNew{{"name", scenario["name"_L1]}};

        const auto musicElements = scenario["music_elements"_L1].toArray();
        auto musicElementsNew = convertMusicElements(musicElements);

        const auto spotifyElements = scenario["spotify_elements"_L1].toArray();
        convertSpotifyElements(spotifyElements, musicElementsNew);

        const auto radioElements = scenario["radio_elements"_L1].toArray();
        const auto radioElementsNew = convertRadioElements(radioElements);

        const auto soundElements = scenario["sound_elements"_L1].toArray();
        const auto soundElementsNew = convertSoundElements(soundElements);

        const auto subScenarios = scenario["scenarios"_L1].toArray();
        const auto subScenariosNew = convertScenarios(subScenarios);

        scenarioNew.insert(u"music_elements"_s, musicElementsNew);
        scenarioNew.insert(u"radio_elements"_s, radioElementsNew);
        scenarioNew.insert(u"sound_elements"_s, soundElementsNew);
        scenarioNew.insert(u"scenarios"_s, subScenariosNew);

        scenariosNew.append(scenarioNew);
    }

    return scenariosNew;
}

auto AudioProjectUpgrader::convertMusicElements(const QJsonArray &elements) -> QJsonArray
{
    QJsonArray elementsNew;

    for (const auto &element : elements)
    {
        QJsonObject elementNew{{"name", element.toObject()["name"]},
                               {"icon", element.toObject()["icon"]},
                               {"mode", element.toObject()["mode"]}};

        QJsonArray filesNew;

        foreach (const auto &file, element.toObject()[QStringLiteral("files")].toArray())
        {
            filesNew.append(QJsonObject{{"url", file.toString()}, {"source", 0}});
        }

        elementNew.insert(QStringLiteral("files"), filesNew);
        elementsNew.append(elementNew);
    }

    return elementsNew;
}

void AudioProjectUpgrader::convertSpotifyElements(const QJsonArray &elements, QJsonArray &musicElementsNew)
{
    for (const auto &element : elements)
    {
        QJsonObject elementNew{
            {"name", element.toObject()["name"]},
            {"icon", element.toObject()["icon"]},
        };

        QJsonArray filesNew;
        filesNew.append(QJsonObject{{"url", element.toObject()["id"]}, {"source", 2}});

        elementNew.insert(QStringLiteral("files"), filesNew);
        musicElementsNew.append(elementNew);
    }
}

auto AudioProjectUpgrader::convertSoundElements(const QJsonArray &elements) -> QJsonArray
{
    // same as music elements
    return convertMusicElements(elements);
}

auto AudioProjectUpgrader::convertRadioElements(const QJsonArray &elements) -> QJsonArray
{
    QJsonArray elementsNew;

    for (const auto &element : elements)
    {
        QJsonObject elementNew{{"name", element.toObject()["name"]}, {"icon", element.toObject()["icon"]}, {"mode", 0}};

        QJsonArray filesNew;
        filesNew.append(
            QJsonObject{{"url", element.toObject()["url"]}, {"source", element.toObject()["local"].toBool() ? 0 : 1}});

        elementNew.insert(QStringLiteral("files"), filesNew);
        elementsNew.append(elementNew);
    }

    return elementsNew;
}
