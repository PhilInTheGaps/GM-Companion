#include "namegenerator.h"
#include "namegeneratorfactory.h"
#include "utils/fileutils.h"
#include "addons/addonmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlContext>
#include <QLoggingCategory>
#include <QFile>
#include <QDir>

Q_LOGGING_CATEGORY(gmNameGenerator, "gm.generators.names")

NameGenerator::NameGenerator(const QQmlApplicationEngine *engine, QObject *parent)
    : AbstractTool(parent), a_currentGenerator(nullptr)
{
    qCDebug(gmNameGenerator()) << "Loading ...";

    if (engine)
    {
        engine->rootContext()->setContextProperty("name_generator", this);
    }
}

void NameGenerator::loadData()
{
    if (isDataLoaded()) return;

    isLoading(true);
    loadCategories();
}

void NameGenerator::loadCategories()
{
    qCDebug(gmNameGenerator()) << "Loading categories ...";

    a_categories.clear();
    m_generatorLists.clear();

    for (const auto& fileName : findAllFiles())
    {
        QFile file(fileName);
        qCDebug(gmNameGenerator()) << fileName;

        if (file.open(QIODevice::ReadOnly))
        {
            auto doc = QJsonDocument::fromJson(file.readAll());
            auto name = doc.object()["name"].toString();

            a_categories << name;
            m_generatorLists << NameGeneratorFactory::buildFromJson(this, doc);

            file.close();
        }
        else
        {
            qCWarning(gmNameGenerator()) << "Could not open" << fileName;
        }
    }

    emit categoriesChanged(a_categories);

    setIsDataLoaded(true);
    isLoading(false);
}

auto NameGenerator::findAllFiles() -> QStringList
{
    auto list = findAllFiles(namesPath);

    list.append(findAllAddonFiles());

    return list;
}

auto NameGenerator::findAllFiles(const QString &path) -> QStringList
{
    QDir dir(path);

    if (!dir.exists()) return {};

    auto entries = dir.entryList({"*.json"}, QDir::Files);

    for (int i = 0; i < entries.length(); i++)
    {
        entries[i] = FileUtils::fileInDir(entries[i], path);
    }

    return entries;
}

auto NameGenerator::findAllAddonFiles() -> QStringList
{
    QStringList list;

    for (const auto *addon : AddonManager::instance()->addons())
    {
        if (addon && addon->enabled())
        {
            const auto namesPath = FileUtils::fileInDir(QStringLiteral("names"), addon->path());
            list.append(findAllFiles(namesPath));
        }
    }

    return list;
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

    a_currentGenerator = generators()[index];
    emit currentGeneratorChanged(a_currentGenerator);
    return true;
}

