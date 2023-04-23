#pragma once

#include "src/common/projects/abstractprojectupgraderini.h"
#include <QJsonObject>

class ConverterProjectUpgrader : public AbstractProjectUpgraderINI
{
public:
    explicit ConverterProjectUpgrader();

    auto run() -> QByteArray override;

private:
    [[nodiscard]] auto convertCategory(const QString &category) const -> QJsonObject;
    [[nodiscard]] auto findProjectName() const -> QString;

    [[nodiscard]] static auto categoryNameFromGroup(const QString &group) -> QString;

    static constexpr int OLD_VERSION = 0;
    static constexpr int NEW_VERSION = 2;
};
