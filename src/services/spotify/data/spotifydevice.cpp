#include "spotifydevice.h"

auto SpotifyDevice::fromJson(const QJsonObject &json) -> SpotifyDevice
{
    SpotifyDevice device;
    device.id = json["id"].toString();
    device.isActive = json["is_active"].toBool();
    device.isPrivateSession = json["is_private_session"].toBool();
    device.isRestricted = json["is_restricted"].toBool();
    device.name = json["name"].toString();
    device.type = json["type"].toString();
    device.volumePercent = json["volume_percent"].toInt();
    return device;
}
