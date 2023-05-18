#include "audioprojectupgrader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>

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

    for (const auto &scenario : scenarios)
    {
        QJsonObject scenarioNew{{"name", scenario["name"]}};

        const auto musicElements = scenario[QStringLiteral("music_elements")].toArray();
        auto musicElementsNew = convertMusicElements(musicElements);

        const auto spotifyElements = scenario[QStringLiteral("spotify_elements")].toArray();
        convertSpotifyElements(spotifyElements, musicElementsNew);

        const auto radioElements = scenario[QStringLiteral("radio_elements")].toArray();
        const auto radioElementsNew = convertRadioElements(radioElements);

        const auto soundElements = scenario[QStringLiteral("sound_elements")].toArray();
        const auto soundElementsNew = convertSoundElements(soundElements);

        const auto subScenarios = scenario[QStringLiteral("scenarios")].toArray();
        const auto subScenariosNew = convertScenarios(subScenarios);

        scenarioNew.insert(QStringLiteral("music_elements"), musicElementsNew);
        scenarioNew.insert(QStringLiteral("radio_elements"), radioElementsNew);
        scenarioNew.insert(QStringLiteral("sound_elements"), soundElementsNew);
        scenarioNew.insert(QStringLiteral("scenarios"), subScenariosNew);

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
