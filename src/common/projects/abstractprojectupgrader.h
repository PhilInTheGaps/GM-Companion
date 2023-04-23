#pragma once

#include <QByteArray>

class AbstractProjectUpgrader
{
public:
    explicit AbstractProjectUpgrader(int version);
    virtual ~AbstractProjectUpgrader() = default;
    Q_DISABLE_COPY_MOVE(AbstractProjectUpgrader)

    virtual auto run() -> QByteArray = 0;
    virtual void parse(const QByteArray &data) = 0;

protected:
    [[nodiscard]] virtual auto isProjectCompatible() const -> bool = 0;

    [[nodiscard]] auto version() const -> int
    {
        return m_version;
    }

private:
    int m_version;
};
