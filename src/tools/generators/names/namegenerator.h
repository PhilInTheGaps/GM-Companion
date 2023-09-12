#pragma once

#include "abstractnamegenerator.h"
#include "abstracttool.h"
#include "utils/stringutils.h"
#include <QByteArrayList>
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

class NameGenerator : public AbstractTool
{
    Q_OBJECT
    QML_NAMED_ELEMENT(NameGeneratorTool)
    QML_SINGLETON

    READ_PROPERTY(QStringList, categories)
    READ_PROPERTY2(AbstractNameGenerator *, currentGenerator, nullptr)
    READ_LIST_PROPERTY(AbstractNameGenerator, generators)

public:
    NameGenerator() = delete;
    using AbstractTool::AbstractTool;

    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> NameGenerator *;

    Q_INVOKABLE bool loadCategory(int index);
    Q_INVOKABLE bool loadGenerator(int index);

public slots:
    void loadData() override;

private:
    void loadCategories();
    static auto findAndReadAllFiles() -> QByteArrayList;
    static auto findAndReadAllFiles(const QString &path) -> QByteArrayList;
    static auto findAndReadAllAddonFiles() -> QByteArrayList;

    static constexpr ConstQString internalNamesPath = ":/names";
    static constexpr ConstQString addonNamesPath = "names";

    QList<QList<AbstractNameGenerator *>> m_generatorLists;
};
