#pragma once

#include "convertereditor.h"
#include "project/converterproject.h"
#include "src/common/abstracttool.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJSEngine>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class Addon;

class ConverterTool : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    friend ConverterEditor;

public:
    ConverterTool() = delete;
    explicit ConverterTool(QObject *parent = nullptr);
    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> ConverterTool *;

    Q_INVOKABLE static QString convert(const ConverterUnit *fromUnit, const QString &fromValue, const ConverterUnit *toUnit);

    Q_PROPERTY(ConverterEditor *editor READ editor CONSTANT)
    [[nodiscard]] auto editor() -> ConverterEditor *;

    READ_LIST_PROPERTY(ConverterProject, projects)
    AUTO_PROPERTY_VAL2(ConverterProject *, currentProject, nullptr)
    AUTO_PROPERTY_VAL2(ConverterCategory *, currentCategory, nullptr)
    AUTO_PROPERTY_VAL2(ConverterUnit *, fromUnit, nullptr)
    AUTO_PROPERTY_VAL2(ConverterUnit *, toUnit, nullptr)

public slots:
    void loadData() override;

private slots:
    void onProjectsChanged();
    void onCurrentProjectChanged(const ConverterProject *project);
    void onCurrentCategoryChanged(const ConverterCategory *category);
    void onEditorSavedChanged(bool isSaved);
    void onAddonManagerLoadingChanged(bool isLoading);

private:
    [[nodiscard]] auto loadLocalProjects() -> QList<ConverterProject *>;
    [[nodiscard]] static auto loadLocalProjects(const QStringList &paths, QObject *parent) -> QList<ConverterProject *>;
    [[nodiscard]] static auto loadLocalProject(const QString &path, QObject *parent) -> ConverterProject *;
    [[nodiscard]] static auto loadProject(const QByteArray &data, QObject *parent) -> ConverterProject *;

    void loadAddonProjects();
    void loadAddonProjects(const Addon &addon);

    void forceReloadData();

    [[nodiscard]] static auto textToNumber(QStringView text, bool *ok = nullptr) -> double;

    ConverterEditor m_editor = ConverterEditor(nullptr);
};
