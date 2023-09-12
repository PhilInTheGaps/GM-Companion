#pragma once

#include "addonreleaseinfo.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonDocument>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class Addon : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    enum class Type
    {
        Folder,
        Archive
    };
    Q_ENUM(Type)

    READONLY_PROPERTY(QString, id)
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, shortName)
    AUTO_PROPERTY(QString, version)
    AUTO_PROPERTY(QString, author)
    AUTO_PROPERTY(QString, description)
    AUTO_PROPERTY(QString, path)
    AUTO_PROPERTY_VAL2(bool, enabled, false)
    AUTO_PROPERTY_VAL(bool, isLocal)
    AUTO_PROPERTY_VAL2(bool, isInstalling, false)
    AUTO_PROPERTY_VAL(Type, type)

    Q_PROPERTY(bool isInstalled READ isInstalled NOTIFY pathChanged)
    Q_PROPERTY(bool isUpdateAvailable READ isUpdateAvailable NOTIFY isUpdateAvailableChanged)

public:
    explicit Addon(QObject *parent, QString id, QString name, QString shortName, QString version, QString author,
                   QString description, QString path, bool isLocal, Type type);

    static auto fromJson(QObject *parent, const QJsonDocument &json, QString path, bool isLocal, Type type) -> Addon *;
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
