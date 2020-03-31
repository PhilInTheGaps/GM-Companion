#include "settingstool.h"
#include <QDebug>

SettingsTool::SettingsTool(QObject *parent) : QObject(parent)
{
}

SettingsTool::~SettingsTool()
{
}

void SettingsTool::setLanguage(QString language)
{
    SettingsManager::setLanguage(language);
}

int SettingsTool::currentLanguageIndex()
{
    return languages.indexOf(SettingsManager::getLanguage());
}

int SettingsTool::uiStyleIndex()
{
    return styles.indexOf(SettingsManager::getSetting("uiMode"));
}

void SettingsTool::setUiStyle(QString style)
{
    SettingsManager::setSetting("uiMode", style);
    emit uiStyleChanged();
}

void SettingsTool::setAddonEnabled(QString addon, bool enabled)
{
    SettingsManager::getInstance()->setAddonEnabled(addon, enabled);
}

QString SettingsTool::getPath(QString type)
{
    return SettingsManager::getPath(type);
}

void SettingsTool::setPath(QString type, QString path, QString group)
{
    SettingsManager::setPath(type, path, group);

    if (type == "audio")
    {
        emit audioPathChanged();
    }
    else if (type == "music")
    {
        emit musicPathChanged();
    }
    else if (type == "sounds")
    {
        emit soundPathChanged();
    }
    else if (type == "radio")
    {
        emit radioPathChanged();
    }
    else if (type == "maps")
    {
        emit mapsPathChanged();
    }
    else if (type == "characters")
    {
        emit charactersPathChanged();
    }
    else if (type == "notes")
    {
        emit notesPathChanged();
    }
    else if (type == "shops")
    {
        emit shopPathChanged();
    }
    else if (type == "resources")
    {
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

QString SettingsTool::getCreditsPage()
{
    QString content;

    QFile f(":/credits.html");

    if (f.open(QIODevice::ReadOnly))
    {
        content = f.readAll();
        f.close();
    }

    return content;
}
