#pragma once

#include "abstractnamegenerator.h"
#include "abstracttool.h"
#include "utils/stringutils.h"
#include <QByteArrayList>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QStringList>

class NameGenerator : public AbstractTool
{
    Q_OBJECT

    READ_PROPERTY(QStringList, categories)
    READ_PROPERTY(AbstractNameGenerator *, currentGenerator)
    AUTO_PROPERTY(QList<AbstractNameGenerator *>, generators)

public:
    explicit NameGenerator(const QQmlApplicationEngine *engine, QObject *parent = nullptr);

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
