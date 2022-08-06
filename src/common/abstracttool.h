#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

#include <QObject>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AbstractTool : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTool(QObject *parent = nullptr);

    AUTO_PROPERTY(bool, isLoading)

public slots:
    virtual void loadData() = 0;

protected:
    bool isDataLoaded() const { return m_isDataLoaded; }
    void setIsDataLoaded(bool value) { m_isDataLoaded = value; }

private:
    bool m_isDataLoaded = false;
};

#endif // ABSTRACTTOOL_H
