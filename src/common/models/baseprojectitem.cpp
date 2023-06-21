#include "baseprojectitem.h"

BaseProjectItem::BaseProjectItem(const QString &name, QObject *parent)
    : QObject(parent), a_name(name), a_originalName(name)
{
    connect(this, &BaseProjectItem::nameChanged, this, &BaseProjectItem::onNameChanged);
    connect(this, &BaseProjectItem::wasRenamedChanged, this, &BaseProjectItem::onWasRenamedChanged);
}

void BaseProjectItem::onNameChanged(const QString &name)
{
    Q_UNUSED(name)
    wasRenamed(true);

    emit wasEdited();
}

void BaseProjectItem::onWasRenamedChanged(bool wasRenamed)
{
    if (wasRenamed) return;

    a_originalName = name();
    emit originalNameChanged(a_originalName);
}
