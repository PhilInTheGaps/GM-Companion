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
        return "#f6f7f8";
    }
    else
    {
        return "black";
    }
}

QString ColorScheme::backgroundColor()
{
    if (style == "Dark")
    {
        return "#2e2e2e";
    }
    else
    {
        return "white";
    }
}

QString ColorScheme::toolbarColor()
{
    if (style == "Dark")
    {
        return "#2f3740";
    }
    else
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
    else
    {
        return "white";
    }
}

QString ColorScheme::menuColor()
{
    if (style == "Dark")
    {
        return "#343a43";
    }
    else
    {
        return "#343a43";
    }
}

QString ColorScheme::dividerColor()
{
    if (style == "Dark")
    {
        return "#bec2c5";
    }
    else
    {
        return "#bec2c5";
    }
}

QString ColorScheme::primaryButtonColor()
{
    if (style == "Dark")
    {
        return "#3f4957";
    }
    else
    {
        return "#3f4957";
    }
}

QString ColorScheme::secondaryButtonColor()
{
    if (style == "Dark")
    {
        return "#404752";
    }
    else
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
    else
    {
        return "white";
    }
}
