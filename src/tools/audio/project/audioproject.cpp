#include "audioproject.h"
#include <QDebug>
#include <QJsonArray>
#include <utility>

AudioProject::AudioProject(const QString& name, int version, QList<AudioCategory *>categories, QObject *parent)
    : QObject(parent), m_name(name), m_version(version), m_categories(std::move(categories))
{
    qDebug() << "Initializing AudioProject:" << name << "...";
    qDebug() << "   Project version:" << version;
    qDebug() << "   Categories:" << m_categories.size();

    if (!m_categories.isEmpty()) setCurrentCategory(m_categories.first());

    for (auto *category : m_categories)
    {
        category->setParent(this);
    }
}

AudioProject::AudioProject(QJsonObject object, QObject *parent)
    : QObject(parent)
{
    m_name    = object["name"].toString();
    m_version = object["version"].toInt();

    for (auto category : object["categories"].toArray())
    {
        m_categories.append(new AudioCategory(category.toObject(), m_name, this));
    }

    if (!m_categories.isEmpty()) setCurrentCategory(m_categories.first());
}

/**
 * @brief Serialize project to json.
 */
auto AudioProject::toJson() const -> QJsonObject
{
    QJsonObject root;

    root.insert("name",    m_name);
    root.insert("version", m_version);

    // Save Categories
    QJsonArray categoriesJson;

    for (auto *category : m_categories)
    {
        if (category) categoriesJson.append(category->toJson());
    }

    root.insert("categories", categoriesJson);
    return root;
}

/**
    Get the names of all categories in project
    @return QStringList with category names
 */
auto AudioProject::categoryNames() const -> QStringList
{
    qDebug() << "AudioProject: Getting category names of project" << m_name << "...";
    QStringList names;

    for (auto *category : m_categories)
    {
        names.append(category->name());
    }

    return names;
}

/**
 * @brief Remove (and delete) a category from the project
 */
auto AudioProject::deleteCategory(AudioCategory *category) -> bool
{
    if (!category || !m_categories.contains(category)) return false;

    if (!m_categories.removeOne(category)) return false;

    category->deleteLater();
    emit categoriesChanged();
    return true;
}

/**
 * @brief Set the current category
 * @param name Name of the category
 */
auto AudioProject::setCurrentCategory(const QString &name) -> bool
{
    qDebug() << "AudioProject: Setting current category:" << name << "...";

    for (auto *category : m_categories)
    {
        if (category && category->name() == name)
        {
            setCurrentCategory(category);
            return true;
        }
    }

    return false;
}

/**
 * @brief Set the current category
 */
auto AudioProject::setCurrentCategory(AudioCategory *category) -> bool
{
    // Disconnect old scenario changed signal
    if (currentCategory())
    {
        disconnect(currentCategory(), &AudioCategory::currentScenarioChanged,
                   this, &AudioProject::currentScenarioChanged);
    }

    m_currentCategory = category;
    emit currentCategoryChanged();
    emit currentScenarioChanged();

    if (category)
    {
        // Connect new scenario changed signal
        connect(category, &AudioCategory::currentScenarioChanged,
                   this, &AudioProject::currentScenarioChanged);
        return true;
    }

    return false;
}

auto AudioProject::addCategory(AudioCategory *category) -> bool
{
    if (!category) return false;

    m_categories.append(category);
    emit categoriesChanged();
    return true;
}

/**
 * @brief Get the current scenario of the current category
 */
AudioScenario *AudioProject::currentScenario() const
{
    if (!currentCategory()) return nullptr;

    return currentCategory()->currentScenario();
}

/**
 * @brief Get a list of all elements within this project.
 */
auto AudioProject::elements() const -> QList<AudioElement *>
{
    QList<AudioElement *> list;

    for (auto category : m_categories)
    {
        if (category) list.append(category->elements());
    }

    return list;
}
