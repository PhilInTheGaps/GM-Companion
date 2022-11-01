#include "abstracttool.h"

AbstractTool::AbstractTool(QObject *parent) : QObject(parent), a_isLoading(false)
{

}

bool AbstractTool::isDataLoaded() const
{
    return m_isDataLoaded;
}

void AbstractTool::setIsDataLoaded(bool value)
{
    m_isDataLoaded = value;
}
