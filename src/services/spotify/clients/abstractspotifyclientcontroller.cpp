#include "abstractspotifyclientcontroller.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "thirdparty/o2/src/o0globals.h"
#include "spotify/spotify.h"
#include "../config.h"

#include <QNetworkRequest>
#include <QLoggingCategory>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRandomGenerator>

Q_LOGGING_CATEGORY(gmSpotifyClientController, "gm.service.spotify.clients.abstract")

using namespace AsyncFuture;

AbstractSpotifyClientController::AbstractSpotifyClientController(QObject *parent, const QLoggingCategory &loggingCategory)
    : QObject{parent}, a_status(new ServiceStatus(this)), m_loggingCategory(loggingCategory)
{
    generateDeviceName();
}

auto AbstractSpotifyClientController::getDevice(const QString &name) -> QFuture<QSharedPointer<SpotifyDevice>>
{
    const auto callback = [name](QSharedPointer<SpotifyDeviceList> deviceList) {
        for (const auto &device : deviceList->devices)
        {
            if (device->name == name) return device;
        }

        return QSharedPointer<SpotifyDevice>();
    };

    return observe(Spotify::instance()->player->devices()).subscribe(callback).future();
}

void AbstractSpotifyClientController::updateStatus(const ServiceStatus::Type &type, const QString &message)
{
    status()->type(static_cast<int>(type));
    status()->message(message);

    qCDebug(m_loggingCategory) << message;
}

void AbstractSpotifyClientController::setActiveDevice(const SpotifyDevice &device)
{
    qCDebug(m_loggingCategory) << "Found librespot instance" << deviceName() << "but it is inactive, setting as active device ...";

    const auto future = Spotify::instance()->player->transfer({ device.id });
    const auto onReply = [this](RestNetworkReply *reply)
    {
        if (reply->hasError()) updateStatus(ServiceStatus::Error, reply->errorText());
        reply->deleteLater();
    };

    const auto onCancellation = [this]()
    {
        updateStatus(ServiceStatus::Error, tr("Error: Could not start librespot."));
    };

    AsyncFuture::observe(future).subscribe(onReply, onCancellation);
}

void AbstractSpotifyClientController::generateDeviceName()
{
    const auto id = QString(QByteArray::number(QRandomGenerator::system()->generate()).toBase64(QByteArray::OmitTrailingEquals));
    m_deviceName = QString(DEVICE_NAME_TEMPLATE).arg(id);

    qCDebug(gmSpotifyClientController()) << "Device Name:" << deviceName();
}
