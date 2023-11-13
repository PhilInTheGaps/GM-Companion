#pragma once

#include <QDir>
#include <QObject>
#include <QSettings>
#include <QVariant>

namespace Common::Settings
{

constexpr auto DEFAULT_GROUP = QLatin1StringView();

template <typename T> struct Request
{
    Request() = default;
    Request(const QString &identifier, const T &defaultValue, const QString &group = DEFAULT_GROUP)
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
    using QObject::QObject;

    template <typename T> T get(QAnyStringView setting, const T &defaultValue, QAnyStringView group = DEFAULT_GROUP);

    template <typename T> T get(const Request<T> &request);

    template <typename T> void set(QAnyStringView setting, const T &value, QAnyStringView group = DEFAULT_GROUP);

    Q_INVOKABLE bool has(QAnyStringView setting, QAnyStringView group = DEFAULT_GROUP);

    void forceSync();

protected:
    template <typename T> void rename(QAnyStringView currentName, QAnyStringView newName, QAnyStringView group);

    void remove(QAnyStringView setting, QAnyStringView group);

private:
    QSettings m_settings = QSettings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);
};

template <typename T>
auto AbstractSettingsManager::get(QAnyStringView setting, const T &defaultValue, QAnyStringView group) -> T
{
    m_settings.beginGroup(group);
    QVariant variant = m_settings.value(setting, defaultValue);
    m_settings.endGroup();
    return variant.value<T>();
}

template <typename T> auto AbstractSettingsManager::get(const Request<T> &request) -> T
{
    return get(request.identifier, request.defaultValue, request.group);
}

template <typename T> void AbstractSettingsManager::set(QAnyStringView setting, const T &value, QAnyStringView group)
{
    m_settings.beginGroup(group);
    m_settings.setValue(setting, value);
    m_settings.endGroup();
}

template <typename T>
void AbstractSettingsManager::rename(QAnyStringView currentName, QAnyStringView newName, QAnyStringView group)
{
    if (const auto value = get<T>(currentName, T(), group); !value.isEmpty())
    {
        set(newName, value, group);
    }
    remove(currentName, group);
}

} // namespace Common::Settings
