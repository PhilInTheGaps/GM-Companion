#include "namegenerator.h"
#include "addons/addon_reader/addonreader.h"
#include "addons/addonmanager.h"
#include "namegeneratorfactory.h"
#include "utils/fileutils.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QQmlContext>

Q_LOGGING_CATEGORY(gmNameGenerator, "gm.generators.names")

NameGenerator::NameGenerator(const QQmlApplicationEngine *engine, QObject *parent)
    : AbstractTool(parent), a_currentGenerator(nullptr)
{
    if (engine)
    {
        engine->rootContext()->setContextProperty(QStringLiteral("name_generator"), this);
    }
}

void NameGenerator::loadData()
{
    if (isDataLoaded()) return;

    qCDebug(gmNameGenerator()) << "Loading ...";

    isLoading(true);
    loadCategories();
}

void NameGenerator::loadCategories()
{
    qCDebug(gmNameGenerator()) << "Loading categories ...";

    a_categories.clear();
    m_generatorLists.clear();

    const auto allFileData = findAndReadAllFiles();

    for (const auto &data : allFileData)
    {
        auto doc = QJsonDocument::fromJson(data);
        auto name = doc.object()[QStringLiteral("name")].toString();

        a_categories << name;
        m_generatorLists << NameGeneratorFactory::buildFromJson(this, doc);
    }

    emit categoriesChanged(a_categories);

    setIsDataLoaded(true);
    isLoading(false);
}

auto NameGenerator::findAndReadAllFiles() -> QByteArrayList
{
    auto list = findAndReadAllFiles(internalNamesPath);

    list.append(findAndReadAllAddonFiles());

    return list;
}

auto NameGenerator::findAndReadAllFiles(const QString &path) -> QByteArrayList
{
    QDir dir(path);

    if (!dir.exists()) return {};

    const auto entries = dir.entryList({"*.json"}, QDir::Files);

    QByteArrayList result;
    result.reserve(entries.length());

    for (int i = 0; i < entries.length(); i++)
    {
        QFile file(FileUtils::fileInDir(entries[i], path));

        if (file.open(QIODevice::ReadOnly))
        {
            result << file.readAll();
            file.close();
        }
        else
        {
            qCWarning(gmNameGenerator()) << "Could not open" << file.fileName();
        }
    }

    return result;
}

auto NameGenerator::findAndReadAllAddonFiles() -> QByteArrayList
{
    QByteArrayList result;
    const auto addons = AddonManager::instance()->addons();

    for (const auto *addon : addons)
    {
        if (addon && addon->isInstalledAndEnabled())
        {
            AddonReader reader(*addon);
            const auto files = reader.findAllFiles(addonNamesPath, {"*.json"});

            for (const auto &file : files)
            {
                result << reader.readFile(FileUtils::fileInDir(file, addonNamesPath));
            }
        }
    }

    return result;
}

auto NameGenerator::loadCategory(int index) -> bool
{
    if (index < 0 || index >= m_generatorLists.length())
    {
        generators({});
        return false;
    }

    generators(m_generatorLists[index]);
    return true;
}

auto NameGenerator::loadGenerator(int index) -> bool
{
    if (index < 0 || index >= generators().length())
    {
        a_currentGenerator = nullptr;
        emit currentGeneratorChanged(nullptr);
        return false;
    }

    a_currentGenerator = qAsConst(a_generators)[index];
    emit currentGeneratorChanged(a_currentGenerator);
    return true;
}
