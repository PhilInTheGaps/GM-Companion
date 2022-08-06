#include "namegenerator.h"
#include "namegeneratorfactory.h"
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

    for (const auto& file : findAllFiles())
    {
        QFile f(QString("%1/%2").arg(namesPath, file));

        qCDebug(gmNameGenerator()) << file;

        if (f.open(QIODevice::ReadOnly))
        {
            auto doc = QJsonDocument::fromJson(f.readAll());
            auto name = doc.object()["name"].toString();

            a_categories << name;
            m_generatorLists << NameGeneratorFactory::buildFromJson(this, doc);

            f.close();
        }
    }

    emit categoriesChanged(a_categories);

    setIsDataLoaded(true);
    isLoading(false);
}

auto NameGenerator::findAllFiles() -> QStringList
{
    QDir dir(namesPath);

    return dir.entryList(QDir::Files);
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

