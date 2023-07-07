#include "abstractspotifyclientcontroller.h"
#include "../config.h"
#include "spotify/spotify.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QNetworkRequest>
#include <QRandomGenerator>

Q_LOGGING_CATEGORY(gmSpotifyClientController, "gm.service.spotify.clients.abstract")

AbstractSpotifyClientController::AbstractSpotifyClientController(QObject *parent,
                                                                 const QLoggingCategory &loggingCategory)
    : QObject{parent}, a_status(new ServiceStatus(this)), m_loggingCategory(loggingCategory)
{
    generateDeviceName(false);
}

auto AbstractSpotifyClientController::getDevice(const QString &name) -> QFuture<QSharedPointer<SpotifyDevice>>
{
    const auto callback = [name](QSharedPointer<SpotifyDeviceList> deviceList) {
        foreach (const auto &device, deviceList->devices)
        {
            if (device->name == name) return device;
        }

        return QSharedPointer<SpotifyDevice>();
    };

    return Spotify::instance()->player->devices().then(Spotify::instance(), callback);
}

void AbstractSpotifyClientController::updateStatus(ServiceStatus::Type type, const QString &message)
{
    a_status->type(type);
    a_status->message(message);

    qCDebug(m_loggingCategory) << message;
}

void AbstractSpotifyClientController::setActiveDevice(const SpotifyDevice &device)
{
    qCDebug(m_loggingCategory) << "Found librespot instance" << deviceName()
                               << "but it is inactive, setting as active device ...";

    auto future = Spotify::instance()->player->transfer({device.id});
    const auto onReply = [this](RestNetworkReply *reply) {
        if (reply->hasError()) updateStatus(ServiceStatus::Type::Error, reply->errorText());
        reply->deleteLater();
    };

    const auto onCancellation = [this]() {
        updateStatus(ServiceStatus::Type::Error, tr("Error: Could not start librespot."));
    };

    future.then(this, onReply).onCanceled(this, onCancellation);
}

void AbstractSpotifyClientController::generateDeviceName(bool makeUnique)
{
    if (makeUnique)
    {
        const auto id = QString(
            QByteArray::number(QRandomGenerator::system()->generate()).toBase64(QByteArray::OmitTrailingEquals));
        m_deviceName = QString(DEVICE_NAME_TEMPLATE).arg(id);
    }
    else
    {
        m_deviceName = DEVICE_NAME;
    }

    qCDebug(gmSpotifyClientController()) << "Device Name:" << deviceName();
}
