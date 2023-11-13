#pragma once

#include "abstractsettingsmanager.h"
#include <QFuture>
#include <QLocale>
#include <QObject>
#include <QPointer>
#include <QStringList>

namespace Common::Settings
{

constexpr auto PATHS_GROUP = QLatin1StringView("Paths");

class SettingsManager : public AbstractSettingsManager
{
    Q_OBJECT

public:
    static auto instance() -> QPointer<SettingsManager>;

    static auto getPath(QAnyStringView setting, QAnyStringView group = QLatin1StringView()) -> QString;
    static void setPath(QAnyStringView setting, const QString &value, QAnyStringView group = QLatin1StringView());

    static auto getLanguage() -> QLocale;
    static auto getLanguageBcp47() -> QString;
    static auto getLanguageString() -> QString;
    static auto getLanguages() -> QStringList;
    static auto getLanguageNames() -> QStringList;

    static auto getServerUrl(QAnyStringView service, bool hasDefault = true) -> QString;
    static void setServerUrl(const QString &url, QAnyStringView service);

    static auto getPassword(const QString &username, const QString &service) -> QString;
    static auto setPassword(const QString &username, const QString &password, const QString &service) -> bool;

    void setAddonEnabled(QAnyStringView addon, bool enabled);
    auto getIsAddonEnabled(QAnyStringView addon) -> bool;

private:
    using AbstractSettingsManager::AbstractSettingsManager;
    inline static QPointer<SettingsManager> m_instance = nullptr;

    static auto getDefaultPath(QAnyStringView setting, QAnyStringView group = PATHS_GROUP) -> QString;
    static auto getActivePathGroup() -> QString;

    void updateSettings();
};

} // namespace Common::Settings
