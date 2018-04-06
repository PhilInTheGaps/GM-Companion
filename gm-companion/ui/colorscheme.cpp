#include "colorscheme.h"

#include <QDebug>

ColorScheme::ColorScheme(QObject *parent) : QObject(parent)
{
    sManager = new SettingsManager;

    style = sManager->getSetting(Setting::uiMode);
}

QString ColorScheme::textColor()
{
    if (style == "Dark")
    {
        return "white";
    }
    else if (style == "Bright")
    {
        return "black";
    }
}

QString ColorScheme::backgroundColor()
{
    if (style == "Dark")
    {
        return "black";
    }
    else if (style == "Bright")
    {
        return "white";
    }
}

QString ColorScheme::toolbarColor()
{
    if (style == "Dark")
    {
        return "#bdc3c7";
    }
    else if (style == "Bright")
    {
        return "#2f3740";
    }
}

QString ColorScheme::toolbarTextColor()
{
    if (style == "Dark")
    {
        return "white";
    }
    else if (style == "Bright")
    {
        return "white";
    }
}

QString ColorScheme::menuColor()
{
    if (style == "Dark")
    {
        return "#1b2224";
    }
    else if (style == "Bright")
    {
        return "#343a43";
    }
}

QString ColorScheme::dividerColor()
{
    if (style == "Dark")
    {
        return "#262f31";
    }
    else if (style == "Bright")
    {
        return "#bec2c5";
    }
}

QString ColorScheme::primaryButtonColor()
{
    if (style == "Dark")
    {
        return "#262f31";
    }
    else if (style == "Bright")
    {
        return "#3f4957";
    }
}

QString ColorScheme::secondaryButtonColor()
{
    if (style == "Dark")
    {
        return "#262f31";
    }
    else if (style == "Bright")
    {
        return "#404752";
    }
}

QString ColorScheme::buttonTextColor()
{
    if (style == "Dark")
    {
        return "white";
    }
    else if (style == "Bright")
    {
        return "white";
    }
}
