#ifndef SHOPTOOL_H
#define SHOPTOOL_H

#include <QStringList>
#include "tools/abstracttool.h"
#include "shopeditor.h"
#include "shopproject.h"
#include <QQmlApplicationEngine>

class ShopTool : public AbstractTool
{
    Q_OBJECT

public:
    explicit ShopTool(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    ShopEditor* getShopEditor() const { return shopEditor; }

    void findShops();

    Q_PROPERTY(QStringList projects READ projects NOTIFY projectsChanged)
    QStringList projects();
    Q_INVOKABLE void setCurrentProject(int index);

    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    QStringList categories();
    Q_INVOKABLE void setCurrentCategory(int index);

    Q_PROPERTY(QStringList shops READ shops NOTIFY shopsChanged)
    QStringList shops();
    Q_INVOKABLE void setCurrentShop(int index);

    Q_PROPERTY(QString shopName READ shopName NOTIFY currentShopChanged)
    QString shopName() const { if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) return m_currentProject->currentCategory()->currentShop()->name(); else return ""; }

    Q_PROPERTY(QString shopOwner READ shopOwner NOTIFY currentShopChanged)
    QString shopOwner() const { if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) return m_currentProject->currentCategory()->currentShop()->owner(); else return ""; }

    Q_PROPERTY(QString shopDescription READ shopDescription  NOTIFY currentShopChanged)
    QString shopDescription() const { if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) return m_currentProject->currentCategory()->currentShop()->description(); else return ""; }

public slots:
    void loadData() override;

signals:
    void projectsChanged();
    void categoriesChanged();
    void shopsChanged();
    void currentShopChanged();

    void shopNameChanged();
    void shopOwnerChanged();
    void shopDescriptionChanged();

private:
    ShopEditor *shopEditor;
    QQmlApplicationEngine *qmlEngine;
    ItemModel *itemModel;

    QList<ShopProject*> m_projects;
    ShopProject *m_currentProject = nullptr;

private slots:
    void updateItems();
    void shopEditorSaved(QList<ShopProject*> projects);
};

#endif // SHOPTOOL_H
