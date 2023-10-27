#include "spotifydevice.h"

using namespace Qt::Literals::StringLiterals;
using namespace Services;

auto SpotifyDevice::fromJson(const QJsonObject &json) -> SpotifyDevice
{
    SpotifyDevice device;
    device.id = json["id"_L1].toString();
    device.isActive = json["is_active"_L1].toBool();
    device.isPrivateSession = json["is_private_session"_L1].toBool();
    device.isRestricted = json["is_restricted"_L1].toBool();
    device.name = json["name"_L1].toString();
    device.type = json["type"_L1].toString();
    device.volumePercent = json["volume_percent"_L1].toInt();
    return device;
}

auto SpotifyDevice::fromJson(const QJsonArray &json) -> SpotifyDeviceList
{
    std::vector<SpotifyDevice> devices;
    devices.reserve(json.count());

    for (const auto &entry : json)
    {
        devices.push_back(SpotifyDevice::fromJson(entry.toObject()));
    }

    return {devices};
}
