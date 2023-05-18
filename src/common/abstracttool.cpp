#include "abstracttool.h"

auto AbstractTool::isDataLoaded() const -> bool
{
    return m_isDataLoaded;
}

void AbstractTool::setIsDataLoaded(bool value)
{
    m_isDataLoaded = value;
}
