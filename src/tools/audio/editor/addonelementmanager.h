#pragma once

#include "src/addons/addon.h"
#include "src/common/abstracttool.h"
#include "src/tools/audio/project/audioproject.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QMap>
#include <QObject>

class AddonElementManager : public AbstractTool
{
    Q_OBJECT
    READ_PROPERTY(QList<Addon *>, addons)
    READ_PROPERTY(QList<AudioProject *>, projects)
    AUTO_PROPERTY_VAL2(int, currentIndex, -1)

public:
    explicit AddonElementManager(QObject *parent = nullptr);

public slots:
    void loadData() override;

private slots:
    void onInstalledAddonsChanged(const QList<Addon *> &addons);
    void onCurrentIndexChanged(int index);
    void onCurrentScenarioChanged(AudioScenario *scenario) const;
    void onProjectsChanged(QList<AudioProject *> projects) const;

private:
    void loadAddonProjects(const Addon &addon);
    static void removeUnsupportedElementsFromProject(AudioProject &project);
    static void removeUnsupportedElementsFromCategory(AudioCategory &category);
    static void removeUnsupportedElementsFromScenario(AudioScenario &scenario);
    static void removeEmptySubscenarios(AudioScenario &scenario);

    QMap<QString, QList<AudioProject *>> m_projects;
};
