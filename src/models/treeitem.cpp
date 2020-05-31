#include "treeitem.h"
#include <QDebug>

TreeItem::TreeItem(const QString &name, int depth, bool canToggle, QObject *parent)
    : QObject(parent), m_name(name), m_canToggle(canToggle), m_depth(depth)
{
    auto *parentItem = qobject_cast<TreeItem*>(parent);

    setObjectName(path());

    if (parentItem)
    {
        connect(this, &TreeItem::childItemsChanged, parentItem, &TreeItem::childItemsChanged);
        connect(this, &TreeItem::destroyed, parentItem, &TreeItem::childItemsChanged);
    }
}

QList<QObject *> TreeItem::childItems() const
{
    if (!isOpen()) return QList<QObject *>();

    auto directChildren = children();
    std::sort(directChildren.begin(), directChildren.end(), [](QObject *o1, QObject *o2) {
        const auto *item1 = qobject_cast<TreeItem*>(o1);
        const auto *item2 = qobject_cast<TreeItem*>(o2);

        if (!item1) return true;
        if (!item2) return false;

        if (item1->priority() != item2->priority())
            return item1->priority() < item2->priority();

        return o1->objectName() < o2->objectName();
    });

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

QString TreeItem::path() const
{
    const auto *parentItem = qobject_cast<TreeItem*>(parent());

    auto name = m_name.endsWith('/') ? m_name.left(m_name.length() - 1) : m_name;

    if (parentItem)
    {
        auto path = parentItem->path();

        if (path.isEmpty()) return name;

        return parentItem->path() + '/' + name;
    }

    return name;
}
