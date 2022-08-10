#include "utils.h"

/// Check if a qobject is in a hierarchy below a certain root object
auto Utils::isInHierarchy(const QObject &object, const QObject *root) -> bool
{
    if (!object.parent()) return false;

    if (object.parent() == root) return true;

    return isInHierarchy(*object.parent(), root);
}
