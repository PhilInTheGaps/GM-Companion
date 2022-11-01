#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>

class AbstractTool : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTool(QObject *parent = nullptr);

    bool isDataLoaded() const;

    AUTO_PROPERTY(bool, isLoading)

public slots:
    virtual void loadData() = 0;

protected:
    void setIsDataLoaded(bool value);

private:
    bool m_isDataLoaded = false;
};

#endif // ABSTRACTTOOL_H
