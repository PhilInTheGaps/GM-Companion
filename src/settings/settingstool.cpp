#include "settingstool.h"
#include <QDebug>

SettingsTool::SettingsTool(QObject *parent) : QObject(parent)
{
    sManager = new SettingsManager;
}

void SettingsTool::setLanguage(QString lang)
{
    sManager->setSetting(Setting::language, true, lang);
}

int SettingsTool::currentLanguageIndex()
{
    return languages.indexOf(sManager->getSetting(Setting::language));
}

int SettingsTool::uiStyleIndex()
{
    return styles.indexOf(sManager->getSetting(Setting::uiMode));
}

void SettingsTool::setUiStyle(QString style)
{
    sManager->setSetting(Setting::uiMode, true, style);
    emit uiStyleChanged();
}

void SettingsTool::setAddonEnabled(QString addon, bool enabled)
{
    sManager->setAddonEnabled(addon, enabled);
}

QString SettingsTool::getPath(QString type)
{
    if (type == "audio")
    {
        return sManager->getSetting(Setting::audioPath);
    }
    else if (type == "music")
    {
        return sManager->getSetting(Setting::musicPath);
    }
    else if (type == "sound")
    {
        return sManager->getSetting(Setting::soundPath);
    }
    else if (type == "radio")
    {
        return sManager->getSetting(Setting::radioPath);
    }
    else if (type == "maps")
    {
        return sManager->getSetting(Setting::mapsPath);
    }
    else if (type == "characters")
    {
        return sManager->getSetting(Setting::charactersPath);
    }
    else if (type == "notes")
    {
        return sManager->getSetting(Setting::notesPath);
    }
    else if (type == "shop")
    {
        return sManager->getSetting(Setting::shopPath);
    }
    else if (type == "resources")
    {
        return sManager->getSetting(Setting::resourcesPath);
    }
    else
    {
        return "";
    }
}

void SettingsTool::setPath(QString type, QString path)
{
    if (type == "audio")
    {
        sManager->setSetting(Setting::audioPath, true, path);
        emit audioPathChanged();
    }
    else if (type == "music")
    {
        sManager->setSetting(Setting::musicPath, true, path);
        emit musicPathChanged();
    }
    else if (type == "sound")
    {
        sManager->setSetting(Setting::soundPath, true, path);
        emit soundPathChanged();
    }
    else if (type == "radio")
    {
        sManager->setSetting(Setting::radioPath, true, path);
        emit radioPathChanged();
    }
    else if (type == "maps")
    {
        sManager->setSetting(Setting::mapsPath, true, path);
        emit mapsPathChanged();
    }
    else if (type == "characters")
    {
        sManager->setSetting(Setting::charactersPath, true, path);
        emit charactersPathChanged();
    }
    else if (type == "notes")
    {
        sManager->setSetting(Setting::notesPath, true, path);
        emit notesPathChanged();
    }
    else if (type == "shop")
    {
        sManager->setSetting(Setting::shopPath, true, path);
        emit shopPathChanged();
    }
    else if (type == "resources")
    {
        sManager->setSetting(Setting::resourcesPath, true, path);
        emit resourcesPathChanged();
    }
    else
    {
        qDebug() << "Error: Invalid path setting" << type;
    }
}

void SettingsTool::setCheckForUpdates(bool check)
{
    QSettings settings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);

    settings.beginGroup("Updates");
    settings.setValue("checkForUpdates", check);

    emit checkForUdatesChanged();
}

bool SettingsTool::getCheckForUpdates()
{
    QSettings settings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);

    settings.beginGroup("Updates");
    return settings.value("checkForUpdates", false).toBool();
}
