#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QNetworkReply>
#include <QString>
#include <QNetworkAccessManager>
#include <QFuture>

#include "version.h"

class UpdateManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString newestVersion READ getNewestVersion NOTIFY updateAvailable)

public:
    UpdateManager();
    Q_INVOKABLE void checkForUpdates();
    Q_INVOKABLE QString getNewestVersion() const { return m_newestVersion; }
    Q_INVOKABLE QString getCurrentVersion() const { return CURRENT_VERSION; }

    static auto compareVersions(const QString &v1, const QString &v2) -> bool;

protected:
    static auto findVersionsFromXML(const QByteArray& xml) -> QStringList;
    static auto findNewestVersion(const QStringList& versions) -> QString;

    auto fetchNewestVersion() -> QFuture<QString>;

private:
    QNetworkAccessManager networkManager;

    QString m_feedURL;
    QString m_newestVersion;

    static void checkSslInstallation();

signals:
    void updateAvailable();
    void noUpdateAvailable();
};

#endif // UPDATEMANAGER_H
