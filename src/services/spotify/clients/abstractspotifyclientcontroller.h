#pragma once

#include <QObject>
#include <QFuture>
#include <QStringList>
#include <QJsonArray>
#include <QLoggingCategory>

#include "spotify/data/spotifydevice.h"
#include "servicestatus.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AbstractSpotifyClientController : public QObject
{
    Q_OBJECT
    READ_PROPERTY(ServiceStatus*, status)

public:
    virtual auto start() -> QFuture<bool> = 0;
    virtual auto stop() -> bool = 0;

    [[nodiscard]] auto deviceName() const { return m_deviceName; }

protected:
    explicit AbstractSpotifyClientController(QObject *parent, const QLoggingCategory &loggingCategory);

    [[nodiscard]] static auto getDevice(const QString &name) -> QFuture<QSharedPointer<SpotifyDevice>>;

    void updateStatus(const ServiceStatus::Type& type, const QString& message);

    void setActiveDevice(const SpotifyDevice &device);

    void generateDeviceName(bool makeUnique);

private:
    QString m_deviceName;
    const QLoggingCategory &m_loggingCategory;
};

