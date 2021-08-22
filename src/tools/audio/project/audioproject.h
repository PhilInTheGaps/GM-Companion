#ifndef AUDIOPROJECT_H
#define AUDIOPROJECT_H

#include "audiocategory.h"
#include "utils/utils.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AudioProject : public TreeItem
{
    Q_OBJECT
    Q_PROPERTY(QObject* currentCategory READ currentCategory NOTIFY currentCategoryChanged)
    Q_PROPERTY(QList<QObject*> categories READ categoryObjects NOTIFY categoriesChanged)
    Q_PROPERTY(int categoryIndex READ categoryIndex NOTIFY currentCategoryChanged)
    Q_PROPERTY(QObject* currentScenario READ currentScenario NOTIFY currentScenarioChanged)

public:
    AudioProject(const QString& name, int version, QList<AudioCategory*> categories, QObject *parent = nullptr);
    AudioProject(QJsonObject object, QObject *parent = nullptr);

    QJsonObject toJson() const;

    bool isCheckable() const override { return true; }

    QList<AudioCategory*> categories() const { return m_categories; }
    QList<QObject*> categoryObjects() const { return Utils::toQObjectList(categories()); }
    QStringList categoryNames() const;
    int categoryIndex() const { return m_categories.indexOf(m_currentCategory); }
    bool deleteCategory(AudioCategory *category);
    bool addCategory(AudioCategory* category, bool setAsCurrent = false);

    AudioCategory* currentCategory() const { return m_currentCategory; }
    Q_INVOKABLE bool setCurrentCategory(AudioCategory *category);

    AudioScenario* currentScenario() const;
    QList<AudioElement *> elements() const;

    AUTO_PROPERTY(bool, isSaved)
    READONLY_PROPERTY(int, version)
    AUTO_PROPERTY(bool, wasRenamed)
    AUTO_PROPERTY(QString, oldName)

signals:
    void categoriesChanged();
    void currentCategoryChanged();
    void currentScenarioChanged();

private:
    QList<AudioCategory*> m_categories;
    AudioCategory *m_currentCategory = nullptr;

    void connectSignals();
    void prepareCategory(AudioCategory *category);

private slots:
    void onWasEdited();
};

#endif // AUDIOPROJECT_H
