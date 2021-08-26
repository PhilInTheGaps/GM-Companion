#ifndef SHOPTOOL_H
#define SHOPTOOL_H

#include <QStringList>
#include <QQmlApplicationEngine>

#include "common/abstracttool.h"
#include "shopeditor.h"
#include "shopproject.h"
#include "filesystem_new/file.h"

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
    QString shopName() const;

    Q_PROPERTY(QString shopOwner READ shopOwner NOTIFY currentShopChanged)
    QString shopOwner() const;

    Q_PROPERTY(QString shopDescription READ shopDescription NOTIFY currentShopChanged)
    QString shopDescription() const;

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

    bool isCurrentShopValid() const;
    ItemShop* currentShop() const;

    static constexpr const char* PROJECT_FILE_GLOB = "*.shop";

private slots:
    void onShopFilesFound(Files::FileListResult *result);
    void onShopFileDataReceived(const QVector<Files::FileDataResult*> &results);
    void updateItems();
    void shopEditorSaved(const QList<ShopProject *> &projects);
};

#endif // SHOPTOOL_H
