#pragma once

#include "abstracttool.h"
#include "project/shopproject.h"
#include "results/filedataresult.h"
#include "results/filelistresult.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QtQml/qqmlregistration.h>

class BaseShopTool : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    AUTO_LIST_PROPERTY(ShopProject, projects)
    AUTO_PROPERTY_VAL2(ShopProject *, currentProject, nullptr)

public:
    using AbstractTool::AbstractTool;

public slots:
    void loadData() override;

protected slots:
    void onShopFilesFound(Files::FileListResult *result);
    void onShopFileDataReceived(const std::vector<Files::FileDataResult *> &results);

protected:
    void setFirstProjectAsCurrent();
};
