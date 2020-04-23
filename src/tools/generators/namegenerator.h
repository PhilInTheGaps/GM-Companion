#ifndef NAMEGENERATOR_H
#define NAMEGENERATOR_H

#include <QObject>
#include <QStringList>
#include <QQmlApplicationEngine>

class NameGenerator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)

public:
    explicit NameGenerator(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    Q_INVOKABLE void updateCategories();

    QStringList categories();
    Q_INVOKABLE QString categoryPath(QString category);

    Q_INVOKABLE QStringList categoryNames(QString category);

    Q_INVOKABLE QStringList maleNames(QString category, QString folder, int count);
    Q_INVOKABLE QStringList femaleNames(QString category, QString folder, int count);

signals:
    void categoriesChanged();

private:
    QStringList m_categories;
    QStringList m_categoryPaths;

    QStringList generateNames(QString category, QString folder, QString type, int count);
};

#endif // NAMEGENERATOR_H
