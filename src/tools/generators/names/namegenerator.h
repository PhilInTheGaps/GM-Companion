#ifndef NAMEGENERATOR_H
#define NAMEGENERATOR_H

#include <QObject>
#include <QStringList>
#include <QQmlApplicationEngine>
#include "abstracttool.h"
#include "abstractnamegenerator.h"

class NameGenerator : public AbstractTool
{
    Q_OBJECT

    READ_PROPERTY(QStringList, categories)
    READ_PROPERTY(AbstractNameGenerator*, currentGenerator)
    AUTO_PROPERTY(QList<AbstractNameGenerator*>, generators)

public:
    explicit NameGenerator(const QQmlApplicationEngine *engine, QObject *parent = nullptr);

    Q_INVOKABLE bool loadCategory(int index);
    Q_INVOKABLE bool loadGenerator(int index);

public slots:
    void loadData() override;

private:
    void loadCategories();
    static auto findAllFiles() -> QStringList;
    static auto findAllFiles(const QString& path) -> QStringList;
    static auto findAllAddonFiles() -> QStringList;

    static constexpr const char* namesPath = ":/names";
    QList<QList<AbstractNameGenerator*>> m_generatorLists;

};

#endif // NAMEGENERATOR_H
