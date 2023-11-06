#pragma once

#include "abstractspotifyclientcontroller.h"
#include <QObject>
#include <QProcess>
#include <QPromise>

namespace Services
{

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

    std::unique_ptr<QPromise<bool>> m_hasAuthenticated = nullptr;

private slots:
    void onLibrespotFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onLibrespotError(QProcess::ProcessError error) const;
    void onLibrespotOutputReady();
};

} // namespace Services
