#pragma once

#include "project/converterproject.h"
#include "src/common/abstracttool.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QList>
#include <QObject>
#include <QQmlApplicationEngine>
#include <gsl/gsl>

class ConverterEditor;

class ConverterTool : public AbstractTool
{
    Q_OBJECT

    friend ConverterEditor;

public:
    explicit ConverterTool(const QQmlApplicationEngine *engine, QObject *parent = nullptr);

    Q_INVOKABLE static QString convert(ConverterUnit *fromUnit, const QString &fromValue, ConverterUnit *toUnit);

    READ_PROPERTY(QList<ConverterProject *>, projects)
    AUTO_PROPERTY_VAL2(ConverterProject *, currentProject, nullptr)
    AUTO_PROPERTY_VAL2(ConverterCategory *, currentCategory, nullptr)
    AUTO_PROPERTY_VAL2(ConverterUnit *, fromUnit, nullptr)
    AUTO_PROPERTY_VAL2(ConverterUnit *, toUnit, nullptr)

public slots:
    void loadData() override;

private slots:
    void onProjectsChanged(const QList<ConverterProject *> &projects);
    void onCurrentProjectChanged(ConverterProject *project);
    void onCurrentCategoryChanged(ConverterCategory *category);
    void onEditorSavedChanged(bool isSaved);

private:
    [[nodiscard]] auto loadLocalProjects() -> QList<ConverterProject *>;
    [[nodiscard]] static auto loadLocalProjects(const QStringList &paths, QObject *parent) -> QList<ConverterProject *>;
    [[nodiscard]] static auto loadLocalProject(const QString &path, QObject *parent) -> gsl::owner<ConverterProject *>;
    [[nodiscard]] static auto loadProject(const QByteArray &data, QObject *parent) -> gsl::owner<ConverterProject *>;

    void loadAddonProjects();
    void forceReloadData();

    [[nodiscard]] static auto textToNumber(QString text, bool *ok = nullptr) -> double;

    gsl::owner<ConverterEditor *> m_editor = nullptr;
};
