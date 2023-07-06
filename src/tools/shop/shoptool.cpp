#include "shoptool.h"
#include "utils/utils.h"
#include <QQmlContext>

using namespace Qt::Literals::StringLiterals;

ShopTool::ShopTool(QQmlApplicationEngine *engine, QObject *parent) : BaseShopTool(parent), m_shopEditor(engine)
{
    if (engine)
    {
        engine->rootContext()->setContextProperty(u"shop_tool"_s, this);
    }

    connect(&m_shopEditor, &ShopEditor::projectsSaved, this, &ShopTool::onShopEditorSaved);
    connect(this, &ShopTool::currentProjectChanged, this, &ShopTool::onCurrentProjectChanged);
}

void ShopTool::onCurrentProjectChanged(ShopProject *project)
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

void ShopTool::onCurrentShopChanged(ItemShop *shop)
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
