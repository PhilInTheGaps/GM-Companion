#ifndef SHOPTOOL_H
#define SHOPTOOL_H

#include <QObject>
#include <QStringList>

#include "shopeditor.h"
#include "shopproject.h"
#include "managers/filemanager.h"
#include <QQmlApplicationEngine>

class ShopTool : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList projects READ projects NOTIFY projectsChanged)
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(QStringList shops READ shops NOTIFY shopsChanged)

    Q_PROPERTY(QString shopName READ shopName NOTIFY currentShopChanged)
    Q_PROPERTY(QString shopOwner READ shopOwner NOTIFY currentShopChanged)
    Q_PROPERTY(QString shopDescription READ shopDescription  NOTIFY currentShopChanged)

public:
    explicit ShopTool(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent = nullptr);

    ShopEditor* getShopEditor() const { return shopEditor; }

    QStringList projects();
    Q_INVOKABLE void setCurrentProject(int index);

    QStringList categories();
    Q_INVOKABLE void setCurrentCategory(int index);

    QStringList shops();
    Q_INVOKABLE void setCurrentShop(int index);

    QString shopName() const { if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) return m_currentProject->currentCategory()->currentShop()->name(); else return ""; }
    QString shopOwner() const { if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) return m_currentProject->currentCategory()->currentShop()->owner(); else return ""; }
    QString shopDescription() const { if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) return m_currentProject->currentCategory()->currentShop()->description(); else return ""; }

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
    FileManager *fileManager;
    QQmlApplicationEngine *qmlEngine;
    ItemModel *itemModel;

    QList<ShopProject*> m_projects;
    ShopProject *m_currentProject = nullptr;

private slots:
    void projectsReceived(QList<ShopProject*> projects);
    void updateItems();
    void shopEditorSaved(QList<ShopProject*> projects);
};

#endif // SHOPTOOL_H
