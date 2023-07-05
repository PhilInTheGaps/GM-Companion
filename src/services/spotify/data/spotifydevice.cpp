#include "spotifydevice.h"

auto SpotifyDevice::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyDevice>
{
    auto *device = new SpotifyDevice();
    device->id = json[QStringLiteral("id")].toString();
    device->isActive = json[QStringLiteral("is_active")].toBool();
    device->isPrivateSession = json[QStringLiteral("is_private_session")].toBool();
    device->isRestricted = json[QStringLiteral("is_restricted")].toBool();
    device->name = json[QStringLiteral("name")].toString();
    device->type = json[QStringLiteral("type")].toString();
    device->volumePercent = json[QStringLiteral("volume_percent")].toInt();
    return QSharedPointer<SpotifyDevice>(device);
}

auto SpotifyDevice::fromJson(const QJsonArray &json) -> QSharedPointer<SpotifyDeviceList>
{
    QList<QSharedPointer<SpotifyDevice>> devices;
    devices.reserve(json.count());

    for (const auto &entry : json)
    {
        devices << SpotifyDevice::fromJson(entry.toObject());
    }

    return QSharedPointer<SpotifyDeviceList>(new SpotifyDeviceList{devices});
}
