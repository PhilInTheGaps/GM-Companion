#pragma once

#include "servicestatus.h"
#include "spotify/data/spotifydevice.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QFuture>
#include <QJsonArray>
#include <QLoggingCategory>
#include <QObject>
#include <QStringList>

namespace Services
{

class AbstractSpotifyClientController : public QObject
{
    Q_OBJECT
    READ_PROPERTY(Status *, status)

public:
    virtual auto start() -> QFuture<bool> = 0;
    virtual auto stop() -> bool = 0;

    [[nodiscard]] auto deviceName() const
    {
        return m_deviceName;
    }

protected:
    explicit AbstractSpotifyClientController(QObject *parent, const QLoggingCategory &loggingCategory);

    [[nodiscard]] static auto getDevice(const QString &name) -> QFuture<SpotifyDevice>;

    void updateStatus(Status::Type type, const QString &message);

    void setActiveDevice(const SpotifyDevice &device);

    void generateDeviceName(bool makeUnique);

private:
    QString m_deviceName;
    const QLoggingCategory &m_loggingCategory;
};

} // namespace Services
