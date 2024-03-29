#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <vector>

namespace Services
{

struct SpotifyDevice;
struct SpotifyDeviceList
{
    std::vector<SpotifyDevice> devices;
};

struct SpotifyDevice
{
    static auto fromJson(const QJsonObject &json) -> SpotifyDevice;
    static auto fromJson(const QJsonArray &json) -> SpotifyDeviceList;

    /// The device ID.
    QString id;

    /// If this device is the currently active device.
    bool isActive;

    /// If this device is currently in a private session.
    bool isPrivateSession;

    /// Whether controlling this device is restricted.
    /// At present if this is "true" then no Web API commands will be accepted by this device.
    bool isRestricted;

    /// The name of the device.
    QString name;

    /// Device type, such as "computer", "smartphone" or "speaker".
    QString type;

    /// The current volume in percent. Between 0 and 100 (inclusive).
    int volumePercent;
};

} // namespace Services
