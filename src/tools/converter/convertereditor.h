#pragma once

#include "project/converterproject.h"
#include "src/common/abstracttool.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QQmlApplicationEngine>

class ConverterEditor : public AbstractTool
{
    Q_OBJECT
public:
    explicit ConverterEditor(const QQmlApplicationEngine *engine, QObject *parent = nullptr);

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

    AUTO_PROPERTY(QList<ConverterProject *>, projects)
    AUTO_PROPERTY_VAL2(ConverterProject *, currentProject, nullptr)
    AUTO_PROPERTY_VAL2(ConverterCategory *, currentCategory, nullptr)

    AUTO_PROPERTY_VAL2(bool, isSaved, true)

public slots:
    void loadData() override;

private slots:
    void onProjectsChanged(const QList<ConverterProject *> &projects);
    void onCurrentProjectChanged(ConverterProject *project);

private:
    static auto getLocalProjectPath() -> QString;

    void madeChanges();
    static void backupV1Projects();
    static void deleteOldProjectFiles();
    static auto saveProject(const ConverterProject *project) -> bool;
};
