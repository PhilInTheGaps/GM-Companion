#pragma once

#include "audiocategory.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QSharedPointer>
#include <QtQml/qqmlregistration.h>

class AudioProject : public TreeItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    READ_LIST_PROPERTY(AudioCategory, categories)
    AUTO_PROPERTY_VAL2(bool, isSaved, true)
    READONLY_PROPERTY(int, version)

public:
    AudioProject(const QString &name, int version, QList<AudioCategory *> categories, QObject *parent = nullptr);
    AudioProject(const AudioProject &other, QObject *parent = nullptr);
    AudioProject(QJsonObject object, QObject *parent = nullptr);
    ~AudioProject() override = default;

    [[nodiscard]] auto toJson() const -> QJsonObject;
    [[nodiscard]] auto isCheckable() const -> bool override
    {
        return true;
    }

    Q_PROPERTY(
        AudioCategory *currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)
    [[nodiscard]] auto currentCategory() const -> AudioCategory *;
    auto setCurrentCategory(AudioCategory *category) -> bool;

    Q_PROPERTY(int categoryIndex READ categoryIndex NOTIFY currentCategoryChanged)
    [[nodiscard]] auto categoryIndex() const -> qsizetype;
    auto deleteCategory(AudioCategory *category) -> bool;
    auto addCategory(AudioCategory *category, bool setAsCurrent = false) -> bool;
    [[nodiscard]] auto containsCategory(const QString &name) const -> bool;

    Q_PROPERTY(AudioScenario *currentScenario READ currentScenario NOTIFY currentScenarioChanged)
    [[nodiscard]] auto currentScenario() const -> AudioScenario *;
    [[nodiscard]] auto elements() const -> QList<AudioElement *>;

signals:
    void currentScenarioChanged(AudioScenario *scenario);
    void currentCategoryChanged(AudioCategory *category);

private:
    AudioCategory *m_currentCategory = nullptr;

    void connectSignals() const;
    void prepareCategory(AudioCategory *category);

private slots:
    void onWasEdited();
};
