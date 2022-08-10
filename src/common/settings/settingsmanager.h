#pragma once

#include <QObject>
#include <QStringList>
#include <QLocale>
#include <QFuture>
#include <QPointer>
#include "abstractsettingsmanager.h"
#include "utils/stringutils.h"

constexpr ConstQString PATHS_GROUP = "Paths";

class SettingsManager : public AbstractSettingsManager
{
    Q_OBJECT

public:
    static auto instance() -> QPointer<SettingsManager>;

    static QString getPath(const QString& setting, QString group = QLatin1String());
    static void setPath(const QString& setting, const QString& value, QString group = QLatin1String());

    static QLocale getLanguage();
    static QString getLanguageString();
    static QStringList getLanguages();
    static QStringList getLanguageNames();

    static QString getServerUrl(const QString &service, bool hasDefault = true);
    static void setServerUrl(const QString& url, const QString& service);

    static QFuture<QString> getPassword(const QString& username, const QString& service);
    static void setPassword(const QString& username, const QString& password, const QString& service);

    void setAddonEnabled(const QString& addon, bool enabled);
    bool getIsAddonEnabled(const QString& addon);

private:
    explicit SettingsManager(QObject *parent) : AbstractSettingsManager(parent) {}
    inline static QPointer<SettingsManager> m_instance = nullptr;

    static auto getDefaultPath(const QString& setting, const QString& group = PATHS_GROUP) -> QString;
    static auto getActivePathGroup() -> QString;

    void updateSettings();
};
