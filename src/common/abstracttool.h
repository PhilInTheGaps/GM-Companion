#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>

class AbstractTool : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    [[nodiscard]] auto isDataLoaded() const -> bool;

    AUTO_PROPERTY_VAL2(bool, isLoading, false)

public slots:
    virtual void loadData() = 0;

protected:
    void setIsDataLoaded(bool value);

private:
    bool m_isDataLoaded = false;
};
