#include "audioproject.h"
#include "src/common/utils/utils.h"
#include <QJsonArray>
#include <QLoggingCategory>
#include <utility>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioProject, "gm.audio.project")

AudioProject::AudioProject(const QString &name, int version, QList<AudioCategory *> categories, QObject *parent)
    : TreeItem(name, 0, true, parent), a_categories(std::move(categories)), a_version(version)
{
    qCDebug(gmAudioProject()) << "Initializing AudioProject:" << name << "[Version:" << version
                              << "Categories:" << a_categories.size() << "]";

    this->name(name);

    if (!a_categories.isEmpty()) setCurrentCategory(a_categories.constFirst());

    foreach (auto *category, a_categories)
    {
        prepareCategory(category);
    }

    connectSignals();
}

AudioProject::AudioProject(const AudioProject &other, QObject *parent)
    : AudioProject(other.name(), other.version(), Utils::copyList(other.categories()), parent)
{
}

AudioProject::AudioProject(QJsonObject object, QObject *parent)
    : TreeItem(object["name"_L1].toString(), 0, true, parent), a_version(object["version"_L1].toInt())
{
    const auto categories = object["categories"_L1].toArray();
    a_categories.reserve(categories.size());

    foreach (const auto &categoryJson, categories)
    {
        auto *category = new AudioCategory(categoryJson.toObject(), name(), this);
        prepareCategory(category);
        a_categories.append(category);
    }

    if (!a_categories.isEmpty()) setCurrentCategory(a_categories.constFirst());
    connectSignals();
}

/**
 * @brief Serialize project to json.
 */
auto AudioProject::toJson() const -> QJsonObject
{
    QJsonObject root;

    root.insert(u"name"_s, name());
    root.insert(u"version"_s, version());

    // Save Categories
    QJsonArray categoriesJson;

    foreach (auto *category, a_categories)
    {
        if (category) categoriesJson.append(category->toJson());
    }

    root.insert(u"categories"_s, categoriesJson);
    return root;
}

/**
 * @brief Remove (and delete) a category from the project
 */
auto AudioProject::deleteCategory(AudioCategory *category) -> bool
{
    if (!category || !a_categories.contains(category)) return false;

    if (!a_categories.removeOne(category)) return false;

    category->deleteLater();
    emit categoriesChanged();
    return true;
}

auto AudioProject::currentCategory() const -> AudioCategory *
{
    return m_currentCategory;
}

/**
 * @brief Set the current category
 */
auto AudioProject::setCurrentCategory(AudioCategory *category) -> bool
{
    // Disconnect old scenario changed signal
    if (currentCategory())
    {
        disconnect(currentCategory(), &AudioCategory::currentScenarioChanged, this,
                   &AudioProject::currentScenarioChanged);
    }

    m_currentCategory = category;
    emit currentCategoryChanged(category);
    emit currentScenarioChanged(category->currentScenario());

    if (category)
    {
        // Connect new scenario changed signal
        connect(category, &AudioCategory::currentScenarioChanged, this, &AudioProject::currentScenarioChanged);
        return true;
    }

    return false;
}

auto AudioProject::categoryIndex() const -> qsizetype
{
    return a_categories.indexOf(m_currentCategory);
}

/**
 * @brief Add a new category to the project
 * @param category The category to be added.
 * @param setAsCurrent Set to true if the category should be set as the current category.
 * @return
 */
auto AudioProject::addCategory(AudioCategory *category, bool setAsCurrent) -> bool
{
    if (!category) return false;

    prepareCategory(category);
    a_categories.append(category);
    emit categoriesChanged();

    if (setAsCurrent)
    {
        return setCurrentCategory(category);
    }

    return true;
}

auto AudioProject::containsCategory(const QString &name) const -> bool
{
    return std::any_of(a_categories.constBegin(), a_categories.constEnd(),
                       [name](const AudioCategory *category) { return category && category->name() == name; });
}

/**
 * @brief Get the current scenario of the current category
 */
auto AudioProject::currentScenario() const -> AudioScenario *
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

    foreach (const auto *category, a_categories)
    {
        if (category) list.append(category->elements());
    }

    return list;
}

auto AudioProject::connectSignals() const -> void
{
    connect(this, &AudioProject::nameChanged, this, &AudioProject::onWasEdited);
    connect(this, &AudioProject::categoriesChanged, this, &AudioProject::onWasEdited);
}

auto AudioProject::prepareCategory(AudioCategory *category) -> void
{
    if (!category) return;
    category->setParent(this);

    connect(category, &AudioCategory::wasEdited, this, &AudioProject::onWasEdited);
    connect(category, &AudioCategory::nameChanged, this, &AudioProject::onWasEdited);
    connect(category, &AudioCategory::scenariosChanged, this, &AudioProject::onWasEdited);
}

auto AudioProject::onWasEdited() -> void
{
    isSaved(false);
}
