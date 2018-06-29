#ifndef NAMEGENERATOR_H
#define NAMEGENERATOR_H

#include "gm-companion/settings/settingsmanager.h"

#include <QObject>
#include <QStringList>

class NameGenerator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)

public:
    explicit NameGenerator(QObject *parent = nullptr);

    Q_INVOKABLE void updateCategories();

    QStringList categories();
    Q_INVOKABLE QString categoryPath(QString category);

    Q_INVOKABLE QStringList categoryNames(QString category);

    Q_INVOKABLE QStringList maleNames(QString category, QString folder, int count);
    Q_INVOKABLE QStringList femaleNames(QString category, QString folder, int count);

signals:
    void categoriesChanged();

private:
    SettingsManager sManager;

    QStringList l_categories;
    QStringList l_categoryPaths;

    QStringList generateNames(QString category, QString folder, QString type, int count);
};

#endif // NAMEGENERATOR_H
