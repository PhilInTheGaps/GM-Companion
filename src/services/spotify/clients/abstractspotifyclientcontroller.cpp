#include "abstractspotifyclientcontroller.h"
#include "../config.h"
#include "spotify/spotify.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QNetworkRequest>
#include <QRandomGenerator>

using namespace Services;

Q_LOGGING_CATEGORY(gmSpotifyClientController, "gm.service.spotify.clients.abstract")

AbstractSpotifyClientController::AbstractSpotifyClientController(QObject *parent,
                                                                 const QLoggingCategory &loggingCategory)
    : QObject{parent}, a_status(new Status(this)), m_loggingCategory(loggingCategory)
{
    generateDeviceName(false);
}

auto AbstractSpotifyClientController::getDevice(const QString &name) -> QFuture<SpotifyDevice>
{
    const auto callback = [name](const SpotifyDeviceList &deviceList) {
        foreach (const auto &device, deviceList.devices)
        {
            if (device.name == name) return device;
        }

        return SpotifyDevice();
    };

    return Spotify::instance()->player.devices().then(callback);
}

void AbstractSpotifyClientController::updateStatus(Status::Type type, const QString &message)
{
    a_status->type(type);
    a_status->message(message);

    qCDebug(m_loggingCategory) << message;
}

void AbstractSpotifyClientController::setActiveDevice(const SpotifyDevice &device)
{
    qCDebug(m_loggingCategory) << "Found librespot instance" << deviceName()
                               << "but it is inactive, setting as active device ...";

    auto future = Spotify::instance()->player.transfer({device.id});
    const auto onReply = [this](const RestReply &reply) {
        if (reply.hasError()) updateStatus(Status::Type::Error, reply.errorText());
    };

    const auto onCancellation = [this]() {
        updateStatus(Status::Type::Error, tr("Error: Could not start librespot."));
    };

    future.then(onReply).onCanceled(onCancellation);
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
