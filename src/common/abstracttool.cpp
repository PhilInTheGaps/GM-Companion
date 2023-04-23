#include "abstracttool.h"

AbstractTool::AbstractTool(QObject *parent) : QObject(parent), a_isLoading(false)
{

}

auto AbstractTool::isDataLoaded() const -> bool
{
    return m_isDataLoaded;
}

void AbstractTool::setIsDataLoaded(bool value)
{
    m_isDataLoaded = value;
}
