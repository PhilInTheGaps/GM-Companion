#pragma once

#include "abstractsettingsmanager.h"
#include "utils/stringutils.h"
#include <QFuture>
#include <QLocale>
#include <QObject>
#include <QPointer>
#include <QStringList>

constexpr ConstQString PATHS_GROUP = "Paths";

class SettingsManager : public AbstractSettingsManager
{
    Q_OBJECT

public:
    static auto instance() -> QPointer<SettingsManager>;

    static auto getPath(const QString &setting, QString group = QLatin1String()) -> QString;
    static void setPath(const QString &setting, const QString &value, QString group = QLatin1String());

    static auto getLanguage() -> QLocale;
    static auto getLanguageBcp47() -> QString;
    static auto getLanguageString() -> QString;
    static auto getLanguages() -> QStringList;
    static auto getLanguageNames() -> QStringList;

    static auto getServerUrl(const QString &service, bool hasDefault = true) -> QString;
    static void setServerUrl(const QString &url, const QString &service);

    static auto getPassword(const QString &username, const QString &service) -> QFuture<QString>;
    static void setPassword(const QString &username, const QString &password, const QString &service);

    void setAddonEnabled(const QString &addon, bool enabled);
    auto getIsAddonEnabled(const QString &addon) -> bool;

private:
    using AbstractSettingsManager::AbstractSettingsManager;
    inline static QPointer<SettingsManager> m_instance = nullptr;

    static auto getDefaultPath(const QString &setting, const QString &group = PATHS_GROUP) -> QString;
    static auto getActivePathGroup() -> QString;

    void updateSettings();
};
