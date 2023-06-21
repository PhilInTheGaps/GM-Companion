#ifndef AUDIOPROJECT_H
#define AUDIOPROJECT_H

#include "audiocategory.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QSharedPointer>

class AudioProject : public TreeItem
{
    Q_OBJECT

public:
    AudioProject(const QString &name, int version, QList<AudioCategory *> categories, QObject *parent = nullptr);
    AudioProject(const AudioProject &other, QObject *parent = nullptr);
    AudioProject(QJsonObject object, QObject *parent = nullptr);

    [[nodiscard]] auto toJson() const -> QJsonObject;
    [[nodiscard]] auto isCheckable() const -> bool override
    {
        return true;
    }

    Q_PROPERTY(
        AudioCategory *currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)
    [[nodiscard]] auto currentCategory() const -> AudioCategory *
    {
        return m_currentCategory;
    }
    auto setCurrentCategory(AudioCategory *category) -> bool;

    Q_PROPERTY(QList<AudioCategory *> categories READ categories NOTIFY categoriesChanged)
    [[nodiscard]] auto categories() const -> QList<AudioCategory *>
    {
        return m_categories;
    }

    Q_PROPERTY(int categoryIndex READ categoryIndex NOTIFY currentCategoryChanged)
    [[nodiscard]] auto categoryIndex() const -> int
    {
        return m_categories.indexOf(m_currentCategory);
    }
    auto deleteCategory(AudioCategory *category) -> bool;
    auto addCategory(AudioCategory *category, bool setAsCurrent = false) -> bool;
    [[nodiscard]] auto containsCategory(const QString &name) const -> bool;

    Q_PROPERTY(AudioScenario *currentScenario READ currentScenario NOTIFY currentScenarioChanged)
    [[nodiscard]] auto currentScenario() const -> AudioScenario *;
    [[nodiscard]] auto elements() const -> QList<AudioElement *>;

    AUTO_PROPERTY_VAL2(bool, isSaved, true)
    READONLY_PROPERTY(int, version)

signals:
    void categoriesChanged();
    void currentScenarioChanged(AudioScenario *scenario);
    void currentCategoryChanged(AudioCategory *category);

private:
    QList<AudioCategory *> m_categories;
    AudioCategory *m_currentCategory = nullptr;

    void connectSignals() const;
    void prepareCategory(AudioCategory *category);

private slots:
    void onWasEdited();
};

#endif // AUDIOPROJECT_H
