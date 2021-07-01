#include "treeitem.h"
#include <QDebug>
#include <utility>

TreeItem::TreeItem(QString name, int depth, bool canToggle, QObject *parent)
    : QObject(parent), m_name(std::move(name)), m_canToggle(canToggle), m_depth(depth)
{
    auto *parentItem = qobject_cast<TreeItem*>(parent);

    setObjectName(path());

    if (parentItem)
    {
        connect(this, &TreeItem::childItemsChanged, parentItem, &TreeItem::childItemsChanged);
        connect(this, &TreeItem::destroyed, parentItem, &TreeItem::childItemsChanged);
        connect(this, &TreeItem::isCheckedChanged, parentItem, &TreeItem::onChildIsCheckedChanged);
    }
}

/**
 * Get all children of the TreeItems that are TreeItems too.
 */
auto TreeItem::childItems() const -> QList<QObject *>
{
    if (!isOpen()) return QList<QObject *>();

    auto directChildren = children();
    std::sort(directChildren.begin(), directChildren.end(), sortChildren);

    QList<QObject *> items;

    for (auto *child : qAsConst(directChildren))
    {
        const auto *item = qobject_cast<TreeItem*>(child);

        if (item)
        {
            items.append(child);
            items.append(item->childItems());
        }
    }

    return items;
}

/**
 * Sort children of a TreeItem.
 * They are sorted first by priority and then alphabetically.
 */
auto TreeItem::sortChildren(QObject *o1, QObject *o2) -> bool
{
    const auto *item1 = qobject_cast<TreeItem*>(o1);
    const auto *item2 = qobject_cast<TreeItem*>(o2);

    if (!item1) return true;
    if (!item2) return false;

    if (item1->priority() != item2->priority())
    {
        return item1->priority() < item2->priority();
    }

    return o1->objectName() < o2->objectName();
}

/**
 * Check or uncheck the item.
 */
void TreeItem::setIsChecked(int isChecked)
{
    if (isChecked == m_isChecked) return;

    for (auto *child : qAsConst(children()))
    {
        auto *item = qobject_cast<TreeItem*>(child);
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

    auto items = childItems();
    for (auto *child : childItems())
    {
        auto *item = qobject_cast<TreeItem*>(child);
        if (Q_LIKELY(item))
        {
            switch (item->isChecked())
            {
            case 0:
                uncheckedChildren++;
                break;
            case 1:
                if (m_isChecked != 1)
                {
                    m_isChecked = 1;
                    emit isCheckedChanged();
                }
                return;
            case 2:
                checkedChildren++;
                break;
            }

            if (checkedChildren > 0 && uncheckedChildren > 0)
            {
                if (m_isChecked != 1)
                {
                    m_isChecked = 1;
                    emit isCheckedChanged();
                }
                return;
            }
        }
    }

    auto newChecked = checkedChildren > 0 ? 2 : 0;

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
    const auto *parentItem = qobject_cast<TreeItem*>(parent());

    auto name = m_name.endsWith('/') ? m_name.left(m_name.length() - 1) : m_name;

    if (parentItem)
    {
        auto path = parentItem->path();

        if (!path.isEmpty()) return QString("%1/%2").arg(path, name);
    }

    return name;
}
