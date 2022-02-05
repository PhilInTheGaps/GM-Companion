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

    [[nodiscard]] auto getDevice(const QString &name) -> QFuture<SpotifyDevice>;
    [[nodiscard]] static auto getDeviceNameList(const QJsonArray &devices) -> QStringList;

    [[nodiscard]] auto deviceNames() const -> QStringList;

    void updateStatus(const ServiceStatus::Type& type, const QString& message);

    void setActiveDevice(const SpotifyDevice &device);

private:
    QStringList m_deviceNames;
    QString m_deviceName;
    const QLoggingCategory &m_loggingCategory;

    void generateDeviceName();
};

