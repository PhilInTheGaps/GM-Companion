#pragma once

#include "src/addons/addon.h"
#include "src/common/abstracttool.h"
#include "src/tools/audio/project/audioproject.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QHash>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class AddonElementManager : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    READ_LIST_PROPERTY(Addon, addons)
    AUTO_PROPERTY_VAL2(int, currentAddonIndex, -1)
    READ_PROPERTY2(Addon *, currentAddon, nullptr)

    READ_LIST_PROPERTY(AudioProject, availableProjects)
    AUTO_PROPERTY_VAL2(int, currentProjectIndex, -1)
    READ_PROPERTY2(AudioProject *, currentProject, nullptr)

public:
    explicit AddonElementManager(QObject *parent = nullptr);

public slots:
    void loadData() override;

private slots:
    void onInstalledAddonsChanged();
    void onCurrentAddonIndexChanged(int index);
    void onCurrentProjectIndexChanged(int index);
    void onCurrentScenarioChanged(AudioScenario *scenario) const;

private:
    void loadAddonProjects(const Addon &addon);
    static void removeUnsupportedElementsFromProject(AudioProject &project);
    static void removeUnsupportedElementsFromCategory(AudioCategory &category);
    static void removeUnsupportedElementsFromScenario(AudioScenario &scenario);
    static void removeEmptySubscenarios(AudioScenario &scenario);

    void clearAddons();
    void clearProjects();

    QHash<QString, QList<AudioProject *>> m_projects;
};

Q_DECLARE_METATYPE(AddonElementManager *)
