#pragma once

#include "project/converterproject.h"
#include "src/common/abstracttool.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

class ConverterEditor : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit ConverterEditor(QObject *parent = nullptr);

    Q_INVOKABLE bool createProject(const QString &name);
    Q_INVOKABLE bool renameProject(ConverterProject *project, const QString &name);
    Q_INVOKABLE bool deleteProject(ConverterProject *project);

    Q_INVOKABLE bool createCategory(const QString &name);
    Q_INVOKABLE bool renameCategory(ConverterCategory *category, const QString &name);
    Q_INVOKABLE bool deleteCategory(ConverterCategory *category);

    Q_INVOKABLE bool createUnit(const QString &name, const QString &value);
    Q_INVOKABLE bool renameUnit(ConverterUnit *unit, const QString &name);
    Q_INVOKABLE bool changeUnitValue(ConverterUnit *unit, const QString &value);
    Q_INVOKABLE bool deleteUnit(ConverterUnit *unit);

    Q_INVOKABLE void save();

    READ_LIST_PROPERTY(ConverterProject, projects)
    AUTO_PROPERTY_VAL2(ConverterProject *, currentProject, nullptr)
    AUTO_PROPERTY_VAL2(ConverterCategory *, currentCategory, nullptr)

    AUTO_PROPERTY_VAL2(bool, isSaved, true)

public slots:
    void loadData() override;

private slots:
    void onProjectsChanged();
    void onCurrentProjectChanged(ConverterProject *project);

private:
    static auto getLocalProjectPath() -> QString;

    void madeChanges();
    static void backupV1Projects();
    static void deleteOldProjectFiles();
    static auto saveProject(const ConverterProject *project) -> bool;
};
