#pragma once

#include "abstractspotifyclientcontroller.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include <QObject>
#include <QProcess>

class LibrespotController : public AbstractSpotifyClientController
{
    Q_OBJECT
public:
    explicit LibrespotController(QObject *parent = nullptr);

    auto start() -> QFuture<bool> override;
    auto stop() -> bool override;
    auto hasStarted() const -> bool;

    void setAsActiveDevice();

private:
    QProcess m_librespotProcess;
    bool m_hasStarted = false;
    bool m_isExitExpected = false;
    bool m_tryAgainIfSettingActiveFails = true;
    static constexpr int TRY_AGAIN_TIMEOUT_MS = 3000;
    static constexpr int PROCESS_TERMINATE_TIMEOUT_MS = 1000;

    void initProcess() const;
    static auto isOtherProcessIsRunning() -> bool;

    static auto getLibrespotPath() -> QString;
    static constexpr auto getLibrespotBinaryName() -> const char *;

    auto getLibrespotArgs(const QString &username) const -> QStringList;

    struct LibrespotInfo
    {
        QString version;
        QString commit;
        QDate buildDate;

        explicit inline operator QString() const
        {
            return QStringLiteral("librespot version %1 (%2) [%3]")
                .arg(version, commit, buildDate.toString(Qt::ISODate));
        }
    };

    auto getLibrespotInfo() -> LibrespotInfo;
    void printOutputAndUpdateStatus(const QString &line);

    AsyncFuture::Deferred<bool> m_hasAuthenticated;

private slots:
    void onLibrespotFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onLibrespotError(QProcess::ProcessError error) const;
    void onLibrespotOutputReady();
};
