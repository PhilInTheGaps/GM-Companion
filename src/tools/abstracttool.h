#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

#include <QObject>

class AbstractTool : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTool(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    virtual void loadData() = 0;

protected:
    bool m_isDataLoaded = false;
};

#endif // ABSTRACTTOOL_H
