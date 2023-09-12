#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAbstractListModel>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QtQml/qqmlregistration.h>

using namespace Qt::Literals::StringLiterals;

class Combatant : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, notes)
    AUTO_PROPERTY_VAL(int, ini)
    AUTO_PROPERTY_VAL(int, health)
    AUTO_PROPERTY_VAL(int, priority)
    AUTO_PROPERTY_VAL(bool, delay)

public:
    explicit Combatant(const QString &name, const QString &notes, int ini, int health, int priority, QObject *parent);
    explicit Combatant(const QString &name, const QString &notes, int ini, int health, int priority, bool delay,
                       QObject *parent);

    QJsonObject toJson() const;
    static Combatant *fromJson(const QJsonObject &json, QObject *parent);

    inline operator QString() const
    {
        return u"%1 (Ini: %2 [%3], Health: %4)"_s.arg(name(), QString::number(ini()), QString::number(priority()),
                                                      QString::number(health()));
    }
};

// Model for QML
class CombatantListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    using QAbstractListModel::QAbstractListModel;

    int rowCount(const QModelIndex &) const override
    {
        return m_items.size();
    }
    QVariant data(const QModelIndex &index, int role) const override;

    void setElements(const QList<Combatant *> &elements);
    void clear();

    void move(int from, int to);
    void append(QObject *item);
    void remove(QObject *item);
    void remove(int index);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<QObject *> m_items = {};
};
