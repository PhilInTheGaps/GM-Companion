#pragma once

#include "utils/stringutils.h"
#include <QDir>
#include <QObject>
#include <QSettings>
#include <QVariant>

constexpr ConstQString DEFAULT_GROUP = "";

template <typename T> struct SettingRequest
{
    SettingRequest() = default;
    SettingRequest(const QString &identifier, const T &defaultValue, const QString &group = DEFAULT_GROUP)
        : identifier(identifier), defaultValue(defaultValue), group(group)
    {
    }

    QString identifier;
    T defaultValue;
    QString group;
};

class AbstractSettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSettingsManager(QObject *parent = nullptr);

    Q_INVOKABLE template <typename T>
    T get(const QString &setting, const T &defaultValue, const QString &group = DEFAULT_GROUP);

    template <typename T> T get(const SettingRequest<T> &request);

    Q_INVOKABLE template <typename T>
    void set(const QString &setting, const T &value, const QString &group = DEFAULT_GROUP);

    Q_INVOKABLE bool has(const QString &setting, const QString &group = DEFAULT_GROUP);

    void forceSync();

protected:
    template <typename T> void rename(const QString &currentName, const QString &newName, const QString &group);

    void remove(const QString &setting, const QString &group);

private:
    QSettings m_settings = QSettings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);
};

template <typename T>
auto AbstractSettingsManager::get(const QString &setting, const T &defaultValue, const QString &group) -> T
{
    m_settings.beginGroup(group);
    QVariant variant = m_settings.value(setting, defaultValue);
    m_settings.endGroup();
    return variant.value<T>();
}

template <typename T> auto AbstractSettingsManager::get(const SettingRequest<T> &request) -> T
{
    return get(request.identifier, request.defaultValue, request.group);
}

template <typename T> void AbstractSettingsManager::set(const QString &setting, const T &value, const QString &group)
{
    m_settings.beginGroup(group);
    m_settings.setValue(setting, value);
    m_settings.endGroup();
}

template <typename T>
void AbstractSettingsManager::rename(const QString &currentName, const QString &newName, const QString &group)
{
    const auto value = get<T>(currentName, T(), group);

    if (!value.isEmpty()) set(newName, value, group);
    remove(currentName, group);
}
