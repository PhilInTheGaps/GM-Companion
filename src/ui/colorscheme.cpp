#include "colorscheme.h"
#include "settings/settingsmanager.h"
#include <QDebug>

ColorScheme::ColorScheme(QObject *parent) : QObject(parent)
{
    stylesheet = new QSettings(":/ui/styles.ini", QSettings::IniFormat);
    style      = SettingsManager::getSetting("uiMode", DEFAULT_UI_MODE);

    stylesheet->beginGroup(style);
}

QString ColorScheme::getColor(QString name, QString defaultValue)
{
    return stylesheet->value(name, defaultValue).toString();
}

void ColorScheme::updateColors()
{
    stylesheet->endGroup();
    style = SettingsManager::getSetting("uiMode", DEFAULT_UI_MODE);
    stylesheet->beginGroup(style);

    emit colorsChanged();
}
