#pragma once

#include "addonreleaseinfo.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonDocument>
#include <QObject>

class Addon : public QObject
{
    Q_OBJECT

public:
    enum AddonType
    {
        Folder,
        Archive
    };
    Q_ENUM(AddonType)

    READONLY_PROPERTY(QString, id)
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, shortName)
    AUTO_PROPERTY(QString, version)
    AUTO_PROPERTY(QString, author)
    AUTO_PROPERTY(QString, description)
    AUTO_PROPERTY(QString, path)
    AUTO_PROPERTY(bool, enabled)
    AUTO_PROPERTY(bool, isLocal)
    AUTO_PROPERTY(bool, isInstalling)
    AUTO_PROPERTY(AddonType, type)

    Q_PROPERTY(bool isInstalled READ isInstalled NOTIFY pathChanged)
    Q_PROPERTY(bool isUpdateAvailable READ isUpdateAvailable NOTIFY isUpdateAvailableChanged)

public:
    explicit Addon(QObject *parent, QString id, QString name, QString shortName, QString version, QString author,
                   QString description, QString path, bool isLocal, AddonType type);

    static auto fromJson(QObject *parent, const QJsonDocument &json, QString path, bool isLocal, Addon::AddonType type)
        -> Addon *;
    static auto fromReleaseInfo(QObject *parent, const AddonReleaseInfo &release) -> Addon *;

    void setReleaseInfo(const AddonReleaseInfo &info);
    void updateWithReleaseInfo();

    [[nodiscard]] auto newVersion() const -> QString;
    [[nodiscard]] auto downloadUrl() const -> QString;

    [[nodiscard]] auto isInstalled() const -> bool;
    [[nodiscard]] auto isInstalledAndEnabled() const -> bool;
    [[nodiscard]] auto isUpdateAvailable() const -> bool;

signals:
    void isUpdateAvailableChanged();

private:
    AddonReleaseInfo m_releaseInfo;
};
