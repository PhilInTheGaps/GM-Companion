#include "addonmanager.h"
#include "src/functions.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSettings>

AddonManager::AddonManager(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Addon Manager ...";
}

/**
 * @brief Find all available addons
 */
void AddonManager::updateAddonList()
{
    m_addonNames.clear();
    m_addonDescriptions.clear();
    m_addonPathNames.clear();
    m_addonEnabledList.clear();

    for (QString path : QStringList({ QDir::homePath() + "/.gm-companion/addons", ":/addons" }))
    {
        for (QString addon : getFolders(path))
        {
            if ((addon != ".") && (addon != ".."))
            {
                QSettings settings(path + "/" + addon + "/addon.ini", QSettings::IniFormat);

                if (settings.value("addons_version", 0).toInt() == 3)
                {
                    m_addonNames.append(settings.value("name", tr("UNKNOWN ADDON")).toString());
                    m_addonDescriptions.append(settings.value("description", "").toString());
                    m_addonPathNames.append(addon);
                    m_addonEnabledList.append(sManager.getIsAddonEnabled(addon));
                }
                else
                {
                    m_addonNames.append(settings.value("name", tr("UNKNOWN ADDON")).toString() + " - " + tr("OUTDATED"));
                    m_addonDescriptions.append(settings.value("description", "").toString());
                    m_addonPathNames.append(addon);
                    m_addonEnabledList.append(false);

                    sManager.setAddonEnabled(addon, false);
                }
            }
        }
    }

    emit addonListChanged();
}
