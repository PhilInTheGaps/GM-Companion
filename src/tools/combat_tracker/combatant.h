#ifndef COMBATANT_H
#define COMBATANT_H

#include <QAbstractListModel>
#include <QJsonObject>
#include <QList>
#include <QObject>

#include "thirdparty/propertyhelper/PropertyHelper.h"

class Combatant : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, notes)
    AUTO_PROPERTY(int, ini)
    AUTO_PROPERTY(int, health)
    AUTO_PROPERTY(int, priority)
    AUTO_PROPERTY(bool, delay)

public:
    explicit Combatant(const QString &name, const QString &notes, int ini, int health, int priority, QObject *parent);
    explicit Combatant(const QString &name, const QString &notes, int ini, int health, int priority, bool delay,
                       QObject *parent);

    QJsonObject toJson() const;
    static Combatant *fromJson(const QJsonObject &json, QObject *parent);

    inline operator QString() const
    {
        return QString("%1 (Ini: %2 [%3], Health: %4)")
            .arg(name(), QString::number(ini()), QString::number(priority()), QString::number(health()));
    }
};

// Model for QML
class CombatantListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    CombatantListModel(QObject *parent = nullptr) : QAbstractListModel(parent)
    {
    }

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

#endif // COMBATANT_H
