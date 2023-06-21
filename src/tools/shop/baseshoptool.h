#pragma once

#include "abstracttool.h"
#include "project/shopproject.h"
#include "results/filedataresult.h"
#include "results/filelistresult.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class BaseShopTool : public AbstractTool
{
    Q_OBJECT

    AUTO_PROPERTY(QList<ShopProject *>, projects)
    AUTO_PROPERTY_VAL2(ShopProject *, currentProject, nullptr)

public:
    using AbstractTool::AbstractTool;

public slots:
    void loadData() override;

protected slots:
    void onShopFilesFound(Files::FileListResult *result);
    void onShopFileDataReceived(const QVector<Files::FileDataResult *> &results);

protected:
    void setFirstProjectAsCurrent();
};
