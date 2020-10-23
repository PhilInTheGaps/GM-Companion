#include "effecttool.h"
#include <QDebug>
#include <QDir>
#include "settings/settingsmanager.h"

EffectTool::EffectTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Effect Tool...";
}

void EffectTool::loadAddons()
{
    QStringList addonFolders = { ":/addons", QDir::homePath() + "/.gm-companion/addons" };

    m_addonPaths.clear();
    m_addons.clear();
    m_addonNames.clear();
    m_effectTypes.clear();
    m_effectList.clear();

    /*
       // Get addons from all paths
       for (QString folder : addonFolders)
       {
        for (QString addon : getFolders(folder))
        {
            if (!addon.contains(".") && !m_addons.contains(addon) &&
               SettingsManager::getInstance()->getIsAddonEnabled(addon) &&
               QFile(folder + "/" + addon + "/combat_effects.ini").exists())
            {
                m_addons.append(addon);
                m_addonPaths.append(folder + "/" + addon);

                QSettings s(folder + "/" + addon + "/addon.ini",
                   QSettings::IniFormat);
                m_addonNames.append(s.value("name", "").toString());
            }
        }
       }*/

    emit addonsChanged();

    if (!m_addons.empty())
    {
        m_currentAddon = m_addons[0];
        loadEffects();
    }
}

void EffectTool::loadEffects()
{
    int index = m_addons.indexOf(m_currentAddon);

    m_effectTypes.clear();

    QString addon     = m_addons[index];
    QString addonPath = m_addonPaths[index];

    if (SettingsManager::getInstance()->getIsAddonEnabled(addon))
    {
        QSettings s(addonPath + "/combat_effects.ini", QSettings::IniFormat);
        s.setIniCodec("UTF-8");
        QStringList types = s.value("types").toStringList();

        for (const QString& type : types)
        {
            qDebug() << type;

            s.beginGroup(type);

            int dice            = s.value("dice").toInt();
            int sides           = s.value("sides").toInt();
            int mod             = s.value("mod").toInt();
            QStringList effects = s.value("effects").toStringList();
            QString     icon    = s.value("icon", "").toString();

            s.endGroup();

            auto *effect = new Effect(type, dice, sides, mod, effects, icon, this);
            m_effectList.append(effect);
            m_effectTypes.append(type);
        }
    }


    qDebug() << effectTypes();
    emit effectTypesChanged();
}

auto EffectTool::getIcon(int index) -> QString
{
    int addonIndex = m_addons.indexOf(m_currentAddon);
    QString path   = m_addonPaths[addonIndex] + "/";

    QString icon = m_effectList[index]->icon();

    if (icon.isEmpty())
    {
        return "";
    }

    if (path.contains(":/"))
    {
        path = path.replace(":/", "qrc:///");
    }
    else
    {
        path = "file:///" + path;
    }

    return path + icon;
}

auto EffectTool::randomEffect(const QString& effectType) -> QString
{
    return m_effectList[m_effectTypes.indexOf(effectType)]->getEffect();
}
