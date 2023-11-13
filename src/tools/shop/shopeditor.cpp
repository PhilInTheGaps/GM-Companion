#include "shopeditor.h"
#include "filesystem/file.h"
#include "filesystem/results/filedataresult.h"
#include "filesystem/results/filelistresult.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include "utils/utils.h"
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QRegularExpression>
#include <algorithm>

using namespace Qt::Literals::StringLiterals;
using namespace Common::Settings;

Q_LOGGING_CATEGORY(gmShopsEditor, "gm.shops.editor")

static constexpr auto ITEM_FILE_GLOB = "*.items";

ShopEditor::ShopEditor(QObject *parent) : BaseShopTool(parent)
{
    m_itemModelGroupProxy.setSourceModel(&m_itemModelGroup);
    m_itemModelGroupProxy.setFilterRole(static_cast<int>(ItemModel::Roles::Category));

    connect(&m_itemEditor, &ItemEditor::itemsSaved, this, &ShopEditor::itemEditorSaved);
    connect(this, &ShopEditor::isLoadingChanged, this, &ShopEditor::onIsLoadingChanged);
    connect(this, &ShopEditor::currentProjectChanged, this, &ShopEditor::onCurrentProjectChanged);
    connect(this, &ShopEditor::currentItemGroupChanged, this, &ShopEditor::onCurrentItemGroupChanged);
    connect(this, &ShopEditor::disabledItemCategoriesChanged, this, &ShopEditor::onDisabledItemCategoriesChanged);
}

auto ShopEditor::itemEditor() -> ItemEditor *
{
    return &m_itemEditor;
}

auto ShopEditor::itemModelShop() -> ItemModel *
{
    return &m_itemModelShop;
}

auto ShopEditor::itemModelGroup() -> QSortFilterProxyModel *
{
    return &m_itemModelGroupProxy;
}

void ShopEditor::loadData()
{
    BaseShopTool::loadData();

    findItems();
    m_itemEditor.loadData();
}

void ShopEditor::findItems()
{
    qCDebug(gmShopsEditor()) << "Finding items ...";

    itemGroups({});

    Files::File::listAsync(SettingsManager::getPath(u"shops"_s), true, false)
        .then([this](Files::FileListResult &&result) { onItemFilesFound(std::move(result)); });
}

void ShopEditor::onItemFilesFound(Files::FileListResult &&result)
{
    const auto &files = result.filesFull(ITEM_FILE_GLOB);

    if (!result.success() || files.isEmpty())
    {
        isLoading(false);
        return;
    }

    Files::File::getDataAsync(files).then([this](const std::vector<Files::FileDataResult> &results) {
        QList<ItemGroup *> groups = {};
        groups.reserve(results.size());

        foreach (const auto &result, results)
        {
            if (!result.success()) continue;

            groups.append(new ItemGroup(tr("Custom"), QJsonDocument::fromJson(result.data()).object(), this));
        }

        itemGroups(groups);
        currentItemGroup(a_itemGroups.isEmpty() ? nullptr : a_itemGroups.constFirst());
    });
}

/// Create a project, category or shop
auto ShopEditor::createThing(const QString &name, Type type) -> bool
{
    switch (type)
    {
    case Type::Project:
        return createProject(name);

    case Type::Category:
        return createCategory(name);

    case Type::Shop:
        return createShop(name);

    default:
        return false;
    }
}

auto ShopEditor::deleteProject(ShopProject *project) -> bool
{
    if (!project) return false;

    auto projects = this->projects();

    if (!projects.removeOne(project)) return false;

    this->projects(projects);

    if (currentProject() == project) setFirstProjectAsCurrent();

    project->deleteLater();
    return true;
}

auto ShopEditor::deleteCategory(ShopCategory *category) const -> bool
{
    if (!currentProject() || !category) return false;

    return currentProject()->deleteCategory(category);
}

auto ShopEditor::moveShop(int positions) const -> bool
{
    qCDebug(gmShopsEditor()) << "Moving shop by" << positions << "position(s) ...";

    if (!isCurrentShopValid()) return false;

    return currentProject()->currentCategory()->moveShop(currentShop(), positions);
}

/// Delete the current shop
auto ShopEditor::deleteShop() const -> bool
{
    if (!isCurrentShopValid()) return false;

    const auto index = currentProject()->currentCategory()->deleteShop(currentShop());

    if (!currentProject()->currentCategory()->shops().isEmpty())
    {
        const auto nextIndex =
            std::clamp(index - 1, static_cast<qsizetype>(0), currentProject()->currentCategory()->shops().size() - 1);
        currentProject()->currentCategory()->currentShop(currentProject()->currentCategory()->shops().at(nextIndex));
    }

    return index >= 0;
}

/// Add a copy of an item to the current shop.
/// Takes the index of the filter proxy model.
auto ShopEditor::addItem(int index) -> bool
{
    if (!isCurrentShopValid()) return false;

    auto proxyIndex = m_itemModelGroupProxy.index(index, 0);
    auto modelIndex = m_itemModelGroupProxy.mapToSource(proxyIndex);
    auto *itemObject = m_itemModelGroup.get(modelIndex.row());
    auto *item = qobject_cast<Item *>(itemObject);

    if (!item) return false;

    currentShop()->addItem(*item);
    m_itemModelShop.append(item);
    return true;
}

/// Remove an item from the current shop
auto ShopEditor::deleteItem(int index) -> bool
{
    if (!isCurrentShopValid()) return false;

    m_itemModelShop.removeRow(index);
    return currentShop()->deleteItem(index);
}

/// Save shops and send copies to shop tool
void ShopEditor::save()
{
    if (projects().isEmpty()) return;

    const auto basePath = SettingsManager::getPath(u"shops"_s);

    std::vector<QFuture<Files::FileResult>> combinator;

    foreach (const auto *project, projects())
    {
        combinator.push_back(Files::File::saveAsync(FileUtils::fileInDir(project->name() + ".shop"_L1, basePath),
                                                    QJsonDocument(project->toJson()).toJson()));
    }

    QtFuture::whenAll(combinator.begin(), combinator.end())
        .then([this](QList<QFuture<Files::FileResult>> &&) {
            isSaved(true);
            emit showInfoBar(tr("Saved!"));
        })
        .onCanceled([this]() { emit showInfoBar(tr("Error: Could not save project(s)!")); });

    sendProjectCopiesToTool();
}

/// Notify UI that changes were made
void ShopEditor::madeChanges()
{
    if (!currentProject()) return;

    isSaved(false);
}

auto ShopEditor::createProject(const QString &name) -> bool
{
    if (name.isEmpty()) return false;

    auto *project = new ShopProject(name, {}, this);
    connectProject(project);

    auto projects = this->projects();
    projects.append(project);
    this->projects(projects);

    setFirstProjectAsCurrent();
    madeChanges();
    return true;
}

auto ShopEditor::createCategory(const QString &name) -> bool
{
    if (!currentProject() || name.isEmpty()) return false;

    auto *category = new ShopCategory(name, {}, this);
    currentProject()->addCategory(category);
    return true;
}

auto ShopEditor::createShop(const QString &name) -> bool
{
    if (!currentProject() || !currentProject()->currentCategory() || name.isEmpty()) return false;

    auto *shop = new ItemShop(name, u""_s, u""_s, {}, this);
    currentProject()->currentCategory()->addShop(shop);
    return true;
}

void ShopEditor::connectProject(const ShopProject *project) const
{
    connect(project, &ShopProject::wasEdited, this, &ShopEditor::onProjectWasEdited);
    connect(project, &ShopProject::currentShopChanged, this, &ShopEditor::onCurrentShopChanged);
}

void ShopEditor::sendProjectCopiesToTool()
{
    QList<ShopProject *> copies;
    copies.reserve(projects().size());

    foreach (const auto *project, projects())
    {
        copies.append(new ShopProject(*project, this));
    }

    emit projectsSaved(copies);
}

auto ShopEditor::isCurrentShopValid() const -> bool
{
    return currentProject() && currentProject()->currentCategory() && currentShop();
}

auto ShopEditor::currentShop() const -> ItemShop *
{
    return currentProject()->currentCategory()->currentShop();
}

/**
 * @brief Set all item categories enabled or not
 */
void ShopEditor::enableAllItemCategories(bool enable)
{
    if (enable)
    {
        a_disabledItemCategories.clear();
    }
    else if (currentItemGroup())
    {
        foreach (const auto *item, currentItemGroup()->items())
        {
            if (item && !a_disabledItemCategories.contains(item->category()))
            {
                a_disabledItemCategories.append(item->category());
            }
        }
    }

    emit disabledItemCategoriesChanged(a_disabledItemCategories);
}

void ShopEditor::setItemCategoryEnabled(const QString &category, bool enable)
{
    if (enable)
    {
        a_disabledItemCategories.removeOne(category);
    }
    else if (!a_disabledItemCategories.contains(category))
    {
        a_disabledItemCategories.append(category);
    }

    emit disabledItemCategoriesChanged(a_disabledItemCategories);
}

auto ShopEditor::isItemCategoryEnabled(const QString &category) const -> bool
{
    return !a_disabledItemCategories.contains(category);
}

/**
 * @brief The item has sent a new custom item group
 * @param group Custom item group
 */
void ShopEditor::itemEditorSaved(ItemGroup *group)
{
    if (!group) return;

    qCDebug(gmShopsEditor()) << "ShopEditor: Received new items from ItemEditor ...";

    if (!itemGroups().isEmpty())
    {
        auto *old = a_itemGroups.takeFirst();
        old->deleteLater();
    }

    a_itemGroups.prepend(group);
    emit itemGroupsChanged(a_itemGroups);

    currentItemGroup(group);
}

void ShopEditor::onProjectWasEdited()
{
    madeChanges();
}

void ShopEditor::onCurrentProjectChanged(const ShopProject *currentProject)
{
    if (!currentProject || !currentProject->currentCategory())
    {
        onCurrentShopChanged(nullptr);
        return;
    }

    onCurrentShopChanged(currentProject->currentCategory()->currentShop());
}

void ShopEditor::onCurrentShopChanged(const ItemShop *currentShop)
{
    if (!currentShop)
    {
        m_itemModelShop.clear();
        return;
    }

    m_itemModelShop.replaceAll(Utils::toQObjectList(currentShop->items()));
}

void ShopEditor::onCurrentItemGroupChanged(const ItemGroup *currentGroup)
{
    if (!currentGroup)
    {
        m_itemModelGroup.clear();
        return;
    }

    m_itemModelGroup.replaceAll(Utils::toQObjectList(currentGroup->items()));
}

void ShopEditor::onDisabledItemCategoriesChanged(const QStringList &categories)
{
    const QRegularExpression ex(u"^(?!(%1)$).*$"_s.arg(categories.join('|')));
    m_itemModelGroupProxy.setFilterRegularExpression(ex);
}

void ShopEditor::onIsLoadingChanged(bool isLoading) const
{
    if (isLoading) return;

    foreach (auto *project, projects())
    {
        connectProject(project);
    }
}
