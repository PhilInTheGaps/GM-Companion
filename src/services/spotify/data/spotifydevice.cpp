#include "spotifydevice.h"

using namespace Qt::Literals::StringLiterals;

auto SpotifyDevice::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyDevice>
{
    auto *device = new SpotifyDevice();
    device->id = json["id"_L1].toString();
    device->isActive = json["is_active"_L1].toBool();
    device->isPrivateSession = json["is_private_session"_L1].toBool();
    device->isRestricted = json["is_restricted"_L1].toBool();
    device->name = json["name"_L1].toString();
    device->type = json["type"_L1].toString();
    device->volumePercent = json["volume_percent"_L1].toInt();
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
