#include "effecttool.h"
#include "settings/settingsmanager.h"
#include <QDir>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmEffectTool, "gm.combat.effects")

EffectTool::EffectTool(QObject *parent) : QObject(parent)
{
    qCDebug(gmEffectTool()) << "Loading Effect Tool...";
}

void EffectTool::loadAddons()
{
    //    QStringList addonFolders = {":/addons", QDir::homePath() + "/.gm-companion/addons"};

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

    QString addon = m_addons[index];
    QString addonPath = m_addonPaths[index];

    if (SettingsManager::instance()->getIsAddonEnabled(addon))
    {
        QSettings s(addonPath + "/combat_effects.ini", QSettings::IniFormat);
        QStringList types = s.value("types").toStringList();

        for (const QString &type : types)
        {
            qCDebug(gmEffectTool()) << type;

            s.beginGroup(type);

            int dice = s.value("dice").toInt();
            int sides = s.value("sides").toInt();
            int mod = s.value("mod").toInt();
            QStringList effects = s.value("effects").toStringList();
            QString icon = s.value("icon", "").toString();

            s.endGroup();

            auto *effect = new Effect(type, dice, sides, mod, effects, icon, this);
            m_effectList.append(effect);
            m_effectTypes.append(type);
        }
    }

    qCDebug(gmEffectTool()) << effectTypes();
    emit effectTypesChanged();
}

auto EffectTool::getIcon(int index) -> QString
{
    int addonIndex = m_addons.indexOf(m_currentAddon);
    QString path = m_addonPaths[addonIndex] + "/";

    QString icon = m_effectList[index]->icon();

    if (icon.isEmpty())
    {
        return u""_s;
    }

    if (path.contains(":/"_L1))
    {
        path = path.replace(":/"_L1, "qrc:///"_L1);
    }
    else
    {
        path = u"file:///"_s + path;
    }

    return path + icon;
}

auto EffectTool::randomEffect(const QString &effectType) -> QString
{
    return m_effectList[m_effectTypes.indexOf(effectType)]->getEffect();
}
