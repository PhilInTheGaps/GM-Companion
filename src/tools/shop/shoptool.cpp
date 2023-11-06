#include "shoptool.h"
#include "utils/utils.h"
#include <QQmlContext>

using namespace Qt::Literals::StringLiterals;

ShopTool::ShopTool(QObject *parent) : BaseShopTool(parent), m_shopEditor(this)
{
    connect(&m_shopEditor, &ShopEditor::projectsSaved, this, &ShopTool::onShopEditorSaved);
    connect(this, &ShopTool::currentProjectChanged, this, &ShopTool::onCurrentProjectChanged);
}

auto ShopTool::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> ShopTool *
{
    Q_UNUSED(jsEngine)
    return new ShopTool(qmlEngine);
}

void ShopTool::onCurrentProjectChanged(const ShopProject *project)
{
    if (m_projectConnection)
    {
        disconnect(m_projectConnection);
    }

    if (!project) return;

    m_projectConnection = connect(project, &ShopProject::currentShopChanged, this, &ShopTool::onCurrentShopChanged);

    if (!project->currentCategory()) return;
    auto *shop = project->currentCategory()->currentShop();
    onCurrentShopChanged(shop);
}

void ShopTool::onCurrentShopChanged(const ItemShop *shop)
{
    if (!shop) return;

    m_itemModel.replaceAll(Utils::toQObjectList(shop->items()));
}

void ShopTool::onShopEditorSaved(const QList<ShopProject *> &projects)
{
    foreach (auto *project, this->projects())
    {
        if (project) project->deleteLater();
    }

    this->projects(projects);
    setFirstProjectAsCurrent();
}

auto ShopTool::itemModel() -> ItemModel *
{
    return &m_itemModel;
}

auto ShopTool::editor() -> ShopEditor *
{
    return &m_shopEditor;
}
