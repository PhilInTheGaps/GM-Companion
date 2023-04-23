#pragma once

#include "abstractprojectupgrader.h"
#include <QSettings>
#include <QTemporaryFile>
#include <memory>

class AbstractProjectUpgraderINI : public AbstractProjectUpgrader
{
public:
    explicit AbstractProjectUpgraderINI(int version);

    void parse(const QByteArray &data) override;

protected:
    [[nodiscard]] auto isProjectCompatible() const -> bool override;

    [[nodiscard]] auto ini() const -> std::shared_ptr<QSettings>
    {
        return m_ini;
    }

private:
    std::shared_ptr<QSettings> m_ini = nullptr;
    QTemporaryFile m_file;
};
