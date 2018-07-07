#include "colorscheme.h"

#include <QDebug>

ColorScheme::ColorScheme(QObject *parent) : QObject(parent)
{
    sManager   = new SettingsManager;
    stylesheet = new QSettings(":/ui/styles.ini", QSettings::IniFormat);
    style      = sManager->getSetting(Setting::uiMode);

    stylesheet->beginGroup(style);
}

QString ColorScheme::getColor(QString name, QString defaultValue)
{
    return stylesheet->value(name, defaultValue).toString();
}
