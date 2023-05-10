#include "treeitem.h"
#include <utility>

TreeItem::TreeItem(QString name, int depth, bool canToggle, QObject *parent)
    : QObject(parent), a_name(std::move(name)), a_canToggle(canToggle), a_depth(depth)
{
    if (auto *parentItem = qobject_cast<TreeItem *>(parent); parentItem)
    {
        connect(this, &TreeItem::childItemsChanged, parentItem, &TreeItem::childItemsChanged);
        connect(this, &TreeItem::destroyed, parentItem, &TreeItem::childItemsChanged);
        connect(this, &TreeItem::isCheckedChanged, parentItem, &TreeItem::onChildIsCheckedChanged);
    }

    connect(this, &TreeItem::isOpenChanged, this, &TreeItem::childItemsChanged);
}

/**
 * Get all children of the TreeItems that are TreeItems too.
 */
auto TreeItem::childItems() const -> QList<QObject *>
{
    if (!isOpen()) return {};

    auto directChildren = children();
    std::sort(directChildren.begin(), directChildren.end(), sortChildren);

    QList<QObject *> items;
    items.reserve(directChildren.size());

    for (auto *child : qAsConst(directChildren))
    {
        const auto *item = qobject_cast<TreeItem *>(child);

        if (item)
        {
            items.append(child);
            items.append(item->childItems());
        }
    }

    return items;
}

auto TreeItem::creatables() const -> QStringList
{
    return {};
}

/**
 * Sort children of a TreeItem.
 * They are sorted first by priority and then alphabetically.
 */
auto TreeItem::sortChildren(QObject *o1, QObject *o2) -> bool
{
    const auto *item1 = qobject_cast<TreeItem *>(o1);
    const auto *item2 = qobject_cast<TreeItem *>(o2);

    if (!item1) return true;
    if (!item2) return false;

    if (item1->priority() != item2->priority())
    {
        return item1->priority() < item2->priority();
    }

    return o1->objectName() < o2->objectName();
}

auto TreeItem::isCheckable() const -> bool
{
    return false;
}

auto TreeItem::isChecked() const -> CheckedState
{
    return m_isChecked;
}

/**
 * Check or uncheck the item.
 */
void TreeItem::setIsChecked(CheckedState isChecked)
{
    if (isChecked == m_isChecked) return;

    for (auto *child : qAsConst(children()))
    {
        auto *item = qobject_cast<TreeItem *>(child);
        if (item) item->setIsChecked(isChecked);
    }

    m_isChecked = isChecked;
    emit isCheckedChanged();
}

/**
 * When a child was (un)checked or children of it were (un)checked
 */
void TreeItem::onChildIsCheckedChanged()
{
    auto checkedChildren = 0;
    auto uncheckedChildren = 0;

    const auto items = childItems();
    for (auto *child : items)
    {
        const auto *item = qobject_cast<TreeItem *>(child);
        if (Q_UNLIKELY(!item)) continue;

        switch (item->isChecked())
        {
        case CheckedState::Unchecked:
            uncheckedChildren++;
            break;
        case CheckedState::PartiallyChecked:
            if (m_isChecked != CheckedState::PartiallyChecked)
            {
                m_isChecked = CheckedState::PartiallyChecked;
                emit isCheckedChanged();
            }
            return;
        case CheckedState::Checked:
        default:
            checkedChildren++;
            break;
        }

        if (checkedChildren > 0 && uncheckedChildren > 0)
        {
            if (m_isChecked != CheckedState::PartiallyChecked)
            {
                m_isChecked = CheckedState::PartiallyChecked;
                emit isCheckedChanged();
            }
            return;
        }
    }

    auto newChecked = checkedChildren > 0 ? CheckedState::Checked : CheckedState::Unchecked;

    if (m_isChecked != newChecked)
    {
        m_isChecked = newChecked;
        emit isCheckedChanged();
    }
}

/**
 * The path of the item in the model.
 */
auto TreeItem::path() const -> QString
{
    const auto *parentItem = qobject_cast<TreeItem *>(parent());
    auto name = this->name().endsWith('/') ? this->name().left(this->name().length() - 1) : this->name();

    if (parentItem)
    {
        auto path = parentItem->path();

        if (!path.isEmpty()) return QStringLiteral("%1/%2").arg(path, name);
    }

    return name;
}

void TreeItem::toggle()
{
    if (canToggle())
    {
        isOpen(!isOpen());
    }
    emit selected();
}

void TreeItem::onChildItemAdded()
{
    emit childItemsChanged();
}

void TreeItem::rename(const QString &newName)
{
    name(newName);
}

void TreeItem::create(const QString &type, const QString &name)
{
    Q_UNUSED(type)
    Q_UNUSED(name)
}

void TreeItem::remove()
{
    this->deleteLater();
}
