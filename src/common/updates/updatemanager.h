#pragma once
#include <QFuture>
#include <QJSEngine>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQmlEngine>
#include <QString>
#include <QtQml/qqmlregistration.h>

class UpdateManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QString newestVersion READ getNewestVersion NOTIFY updateAvailable)

public:
    explicit UpdateManager(QObject *parent = nullptr);
    Q_INVOKABLE void checkForUpdates();
    [[nodiscard]] Q_INVOKABLE QString getNewestVersion() const;
    [[nodiscard]] static Q_INVOKABLE QString getCurrentVersion();

    static auto compareVersions(const QString &v1, const QString &v2) -> bool;

    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> UpdateManager *
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)
        return new UpdateManager(qmlEngine);
    }

protected:
    static auto findVersionsFromXML(const QByteArray &xml) -> QStringList;
    static auto findNewestVersion(const QStringList &versions) -> QString;

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
