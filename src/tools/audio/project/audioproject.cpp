#include "audioproject.h"
#include "src/common/utils/utils.h"
#include <QJsonArray>
#include <QLoggingCategory>
#include <utility>

Q_LOGGING_CATEGORY(gmAudioProject, "gm.audio.project")

AudioProject::AudioProject(const QString &name, int version, QList<AudioCategory *> categories, QObject *parent)
    : TreeItem(name, 0, true, parent), a_isSaved(true), a_version(version), a_wasRenamed(false),
      m_categories(std::move(categories))
{
    qCDebug(gmAudioProject()) << "Initializing AudioProject:" << name << "[Version:" << version
                              << "Categories:" << m_categories.size() << "]";

    this->name(name);

    if (!m_categories.isEmpty()) setCurrentCategory(m_categories.first());

    for (auto *category : qAsConst(m_categories))
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
    : TreeItem(QLatin1String(""), 0, true, parent), a_isSaved(true), a_version(object[QStringLiteral("version")].toInt()), a_wasRenamed(false)
{
    name(object[QStringLiteral("name")].toString());
    const auto categories = object[QStringLiteral("categories")].toArray();
    m_categories.reserve(categories.size());

    foreach (const auto &category, categories)
    {
        auto *object = new AudioCategory(category.toObject(), name(), this);
        prepareCategory(object);
        m_categories.append(object);
    }

    if (!m_categories.isEmpty()) setCurrentCategory(m_categories.first());
    connectSignals();
}

/**
 * @brief Serialize project to json.
 */
auto AudioProject::toJson() const -> QJsonObject
{
    QJsonObject root;

    root.insert(QStringLiteral("name"), name());
    root.insert(QStringLiteral("version"), version());

    // Save Categories
    QJsonArray categoriesJson;

    for (auto *category : m_categories)
    {
        if (category) categoriesJson.append(category->toJson());
    }

    root.insert(QStringLiteral("categories"), categoriesJson);
    return root;
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
    m_categories.append(category);
    emit categoriesChanged();

    if (setAsCurrent)
    {
        return setCurrentCategory(category);
    }

    return true;
}

auto AudioProject::containsCategory(const QString &name) const -> bool
{
    return std::any_of(m_categories.constBegin(), m_categories.constEnd(),
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

    foreach (const auto *category, m_categories)
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
