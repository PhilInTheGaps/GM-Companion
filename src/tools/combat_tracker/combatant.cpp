#include "combatant.h"

Combatant::Combatant(QString name, QString notes, int ini, int health, QObject *parent)
    : QObject(parent), m_name(name), m_notes(notes), m_ini(ini), m_health(health)
{
}

QVariant CombatantListModel::data(const QModelIndex& index, int /*role*/) const
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
}

void CombatantListModel::insert(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.push_front(item);
    endInsertRows();
}

void CombatantListModel::remove(QObject *item)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items.at(i) == item) {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.remove(i);
            endRemoveRows();
            break;
        }
    }
}

QHash<int, QByteArray>CombatantListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void CombatantListModel::clear()
{
    while (m_items.size() > 0)
    {
        remove(m_items[0]);
    }
}

void CombatantListModel::setElements(QList<Combatant *>elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }
}
