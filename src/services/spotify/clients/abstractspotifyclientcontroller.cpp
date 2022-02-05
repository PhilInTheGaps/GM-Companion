#include "abstractspotifyclientcontroller.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "thirdparty/o2/src/o0globals.h"
#include "../spotify.h"
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

auto AbstractSpotifyClientController::getDevice(const QString &name) -> QFuture<SpotifyDevice>
{
    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/devices"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, static_cast<const char*>(O2_MIME_TYPE_JSON));

    const auto callback = [this, name](RestNetworkReply *reply) {
        const auto data = reply->data();
        const auto devices = QJsonDocument::fromJson(data).object().value("devices").toArray();
        reply->deleteLater();

        m_deviceNames = getDeviceNameList(devices);
        qCDebug(gmSpotifyClientController()) << "Found the following active spotify devices:" << m_deviceNames;

        for (const auto &device : devices)
        {
            if (device.toObject()["name"].toString() == name)
            {
                return SpotifyDevice::fromJson(device.toObject());
            }
        }

        return SpotifyDevice();
    };

    return observe(Spotify::getInstance()->get(request)).subscribe(callback).future();
}

auto AbstractSpotifyClientController::getDeviceNameList(const QJsonArray &devices) -> QStringList
{
    QStringList names;

    for (const auto &device : devices)
    {
        names << device.toObject()["name"].toString();
    }

    return names;
}

auto AbstractSpotifyClientController::deviceNames() const -> QStringList
{
    return m_deviceNames;
}

void AbstractSpotifyClientController::updateStatus(const ServiceStatus::Type &type, const QString &message)
{
    status()->type(static_cast<int>(type));
    status()->message(message);

    qCDebug(m_loggingCategory) << message;
}

void AbstractSpotifyClientController::setActiveDevice(const SpotifyDevice &device)
{
    // Make it active.
    QJsonObject parameters
    {
        { "device_ids", QJsonArray { device.id } },
        { "play", false }
    };

    qCDebug(m_loggingCategory) << "Found librespot instance" << deviceName() << "but it is inactive, setting as active device ...";

    const auto url = QUrl("https://api.spotify.com/v1/me/player");
    const auto payload = QJsonDocument(parameters).toJson(QJsonDocument::JsonFormat::Compact);
    const auto future = Spotify::getInstance()->put(url, payload);

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
