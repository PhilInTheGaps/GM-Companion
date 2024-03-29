#include "combatant.h"
#include <utility>

using namespace Qt::Literals::StringLiterals;

Combatant::Combatant(const QString &name, const QString &notes, int ini, int health, int priority, QObject *parent)
    : Combatant(name, notes, ini, health, priority, false, parent)
{
}

Combatant::Combatant(const QString &name, const QString &notes, int ini, int health, int priority, bool delay,
                     QObject *parent)
    : QObject(parent), a_name(name), a_notes(notes), a_ini(ini), a_health(health), a_priority(priority), a_delay(delay)
{
}

auto Combatant::toJson() const -> QJsonObject
{
    return QJsonObject({{"name", name()},
                        {"notes", notes()},
                        {"ini", ini()},
                        {"health", health()},
                        {"priority", priority()},
                        {"delay", delay()}});
}

auto Combatant::fromJson(const QJsonObject &json, QObject *parent) -> Combatant *
{
    return new Combatant(json["name"_L1].toString(), json["notes"_L1].toString(), json["ini"_L1].toInt(),
                         json["health"_L1].toInt(), json["priority"_L1].toInt(), json["delay"_L1].toBool(), parent);
}

auto CombatantListModel::data(const QModelIndex &index, int /*role*/) const -> QVariant
{
    auto *item = m_items.at(index.row());
    return QVariant::fromValue(item);
}

auto CombatantListModel::append(QObject *item) -> void
{
    beginInsertRows(QModelIndex(), static_cast<int>(m_items.length()), static_cast<int>(m_items.length()));
    m_items.push_back(item);
    endInsertRows();
}

auto CombatantListModel::remove(int index) -> void
{
    if (Q_UNLIKELY(index < 0)) return;

    beginRemoveRows(QModelIndex(), index, index);
    m_items.remove(index);
    endRemoveRows();
}

auto CombatantListModel::remove(QObject *item) -> void
{
    const auto index = static_cast<int>(m_items.indexOf(item));
    remove(index);
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
        remove(m_items.constFirst());
    }
}

void CombatantListModel::move(int from, int to)
{
    beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + 1);
    m_items.move(from, to);
    endMoveRows();
}

auto CombatantListModel::setElements(const QList<Combatant *> &elements) -> void
{
    clear();

    foreach (auto *element, elements)
    {
        append(element);
    }
}
