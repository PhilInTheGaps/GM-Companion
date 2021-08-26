#ifndef CHARACTERDSA5VIEWER_H
#define CHARACTERDSA5VIEWER_H

#include "characterviewer.h"
#include "dsa5/dsa5listitem.h"
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class CharacterDSA5Viewer : public CharacterViewer
{
    Q_OBJECT
    Q_PROPERTY(QStringList subCategories READ subCategories NOTIFY categoryChanged)
    Q_PROPERTY(int subCategoryIndex READ subCategoryIndex NOTIFY subCategoryChanged)

    Q_PROPERTY(QString image READ image NOTIFY characterChanged)
    Q_PROPERTY(QStringList profileOverview READ profileOverview NOTIFY characterChanged)
    Q_PROPERTY(QStringList ruleBooks READ ruleBooks NOTIFY characterChanged)
    Q_PROPERTY(QStringList optionalRules READ optionalRules NOTIFY characterChanged)
    Q_PROPERTY(QStringList attributes READ attributes NOTIFY characterChanged)
    Q_PROPERTY(QStringList advantages READ advantages NOTIFY characterChanged)
    Q_PROPERTY(QStringList disadvantages READ disadvantages NOTIFY characterChanged)
    Q_PROPERTY(QStringList special READ special NOTIFY characterChanged)

public:
    explicit CharacterDSA5Viewer(QQmlApplicationEngine *engine, QObject *parent);

    void setCharacter(Character* character);
    void setCurrentCategory(int index);
    int pageIndex() const { return 1; }

    QStringList subCategories() const;
    Q_INVOKABLE void setCurrentSubCategory(int index) { m_subCategoryIndex = index; emit subCategoryChanged(); }
    int subCategoryIndex() const { return m_subCategoryIndex; }

    QString image() const { return m_image; }
    QStringList profileOverview() const { return m_profileOverview; }
    QStringList ruleBooks() const { return m_ruleBooks; }
    QStringList optionalRules() const { return m_optionalRules; }
    QStringList attributes() const { return m_attributes; }
    QStringList advantages() const { return m_advantages; }
    QStringList disadvantages() const { return m_disadvantages; }
    QStringList special() const { return m_special; }

signals:
    void characterChanged();
    void categoryChanged();
    void subCategoryChanged();

private:
    QQmlApplicationEngine *qmlEngine = nullptr;

    DSA5ListModel *talentsModel, *combatModel, *specialModel, *liturgiesModel, *spellsModel, *itemsModel;

    int m_subCategoryIndex = 0;

    QString m_image;
    QStringList m_profileOverview, m_ruleBooks, m_optionalRules, m_attributes, m_advantages, m_disadvantages, m_special;
    QJsonObject m_jsonObject, m_race;
    QJsonObject m_referenceObject;

    void loadCharacterData();
    void loadProfile();
    void loadRules();
    void loadAttributes();
    void loadAdvantages();
    void loadDisadvantages();
    void loadTalents();
    void loadCombat();
    void loadSpecial();
    void loadItems();

    void loadReferenceObject();
    QString jsonString(const QString &key);
    QString jsonMapString(const QString &key, bool shiftIndex = true);
};

#endif // CHARACTERDSA5VIEWER_H
