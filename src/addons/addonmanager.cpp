#include "addonmanager.h"
#include "utils/fileutils.h"
#include "settings/settingsmanager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmAddonManager, "gm.addons.manager")

AddonManager::AddonManager(QObject *parent) : QObject(parent)
{
    load();
}

auto AddonManager::instance() -> AddonManager*
{
    if (!s_instance)
    {
        s_instance = new AddonManager(nullptr);
    }

    return s_instance;
}

auto AddonManager::setAddonEnabled(int index, bool value) -> bool
{
    if (index < 0 || index >= addons().length()) return false;

    auto *addon = addons()[index];
    addon->enabled(value);
    SettingsManager::getInstance()->setAddonEnabled(addon->path(), value);

    return true;
}

void AddonManager::load()
{
    qCDebug(gmAddonManager()) << "Loading ...";

    const auto addonDirs = {
        QStringLiteral(":/addons"),
        QString("%1/.gm-companion/addons").arg(QDir::homePath())
    };

    a_addons.clear();

    for (const auto &dir : addonDirs)
    {
        loadFromDir(dir);
    }

    emit addonsChanged(a_addons);
}

void AddonManager::loadFromDir(const QString &dir)
{
    const auto addonsDir = QDir(dir);
    const auto folders = addonsDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);

    for (const auto& folder : folders)
    {
        const auto path = FileUtils::fileInDir(folder, addonsDir.absolutePath());
        QFile file(FileUtils::fileInDir("addon.json", path));

        if (file.exists() && file.open(QIODevice::ReadOnly))
        {
            auto json = QJsonDocument::fromJson(file.readAll());
            file.close();

            auto *addon = Addon::fromJson(this, json, std::move(path));
            addon->enabled(SettingsManager::getInstance()->getIsAddonEnabled(addon->path()));
            a_addons << addon;
        }
        else if (file.exists())
        {
            qCWarning(gmAddonManager()) << "Could not open" << file.fileName();
        }
    }
}
