#pragma once

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QProcess>

#include "spotifyconnectorlocal.h"
#include "spotifyconnectorserver.h"
#include "spotifynetworkerror.h"
#include "settings/settingsmanager.h"
#include "service.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class Spotify : public Service
{
    Q_OBJECT
public:
    static auto getInstance() -> Spotify*;

    void grant() { m_connector->grantAccess(); }
    [[nodiscard]] auto isGranted() const -> bool { return m_connector->isAccessGranted(); }

    auto get(const QNetworkRequest &request) -> QFuture<RestNetworkReply*>;
    auto get(const QUrl &url) -> QFuture<RestNetworkReply*>;
    auto put(const QUrl &url, const QByteArray &data = "") -> QFuture<RestNetworkReply*>;
    auto post(const QNetworkRequest& request, const QByteArray &data = "") -> QFuture<RestNetworkReply*>;

    AUTO_PROPERTY(QString, username);

public slots:
    void connectService() override;
    void disconnectService() override;

private:
    explicit Spotify(QObject* parent = nullptr);
    inline static Spotify *single = nullptr;

    QNetworkAccessManager *m_networkManager = nullptr;
    RESTServiceConnector *m_connector = nullptr;
    QProcess m_librespotProcess;
    bool isLibrespotRunning = false;

    void handleNetworkError(const RestNetworkReply *reply);
    void handleAccessDenied(const SpotifyNetworkError &error);

    void setDeviceActive();
    void stopLibrespot();

    static auto getLibrespotPath() -> QString;
    static constexpr auto getLibrespotBinaryName() -> const char*;

    void updateConnector();
    auto startLibrespot() -> bool;

signals:
    void authorized();
    void usernameChanged();

private slots:
    void onAccessGranted();
    void onReceivedDevices(const RestNetworkReply *reply);
    void onLibrespotFinished(const int& exitCode, const QProcess::ExitStatus& exitStatus);
    void onLibrespotError(const QProcess::ProcessError& error);
};
