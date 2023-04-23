#pragma once

#include "abstractprojectupgrader.h"
#include <QJsonDocument>

class AbstractProjectUpgraderJSON : public AbstractProjectUpgrader
{
public:
    explicit AbstractProjectUpgraderJSON(int version);
    void parse(const QByteArray &data) override;

protected:
    [[nodiscard]] auto isProjectCompatible() const -> bool override;
    [[nodiscard]] auto json() const -> QJsonDocument;

private:
    QJsonDocument m_json;
};
