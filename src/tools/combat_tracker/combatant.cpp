#include "combatant.h"

#include <utility>

Combatant::Combatant(QString name, QString notes, int ini, int health, int priority, QObject *parent)
    : QObject(parent), m_name(std::move(name)), m_notes(std::move(notes)), m_ini(ini), m_health(health), m_priority(priority)
{
}

auto CombatantListModel::data(const QModelIndex& index, int /*role*/) const -> QVariant
{
    auto *item = m_items.at(index.row());
    return QVariant::fromValue(item);
}

auto CombatantListModel::insert(QObject *item) -> void
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.push_front(item);
    endInsertRows();
}

auto CombatantListModel::remove(QObject *item) -> void
{
    auto index = m_items.indexOf(item);
    if (Q_UNLIKELY(index < 0)) return;

    beginRemoveRows(QModelIndex(), index, index);
    m_items.remove(index);
    endRemoveRows();
}

auto CombatantListModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

auto CombatantListModel::clear() -> void
{
    while (!m_items.empty())
    {
        remove(m_items.first());
    }
}

auto CombatantListModel::setElements(QList<Combatant *>elements) -> void
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }
}
