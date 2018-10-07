#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include <QObject>
#include <QSettings>
#include "gm-companion/settings/settingsmanager.h"

class ColorScheme : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString textColor READ textColor NOTIFY textColorChanged)
    Q_PROPERTY(QString backgroundColor READ backgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QString toolbarColor READ toolbarColor NOTIFY toolbarColorChanged)
    Q_PROPERTY(QString menuColor READ menuColor NOTIFY menuColorChanged)
    Q_PROPERTY(QString dividerColor READ dividerColor NOTIFY dividerColorChanged)
    Q_PROPERTY(QString primaryButtonColor READ primaryButtonColor NOTIFY primaryButtonColorChanged)
    Q_PROPERTY(QString secondaryButtonColor READ secondaryButtonColor NOTIFY secondaryButtonColorChanged)
    Q_PROPERTY(QString toolbarTextColor READ toolbarTextColor NOTIFY toolbarTextColorChanged)
    Q_PROPERTY(QString buttonTextColor READ buttonTextColor NOTIFY buttonTextColorChanged)
    Q_PROPERTY(QString listHeaderBackgroundColor READ listHeaderBackgroundColor NOTIFY listHeaderBackgroundColorChanged)
    Q_PROPERTY(QString listHeaderTextColor READ listHeaderTextColor NOTIFY listHeaderTextColorChanged)
    Q_PROPERTY(QString combatTrackerXType READ combatTrackerXType NOTIFY combatTrackerXTypeChanged)
    Q_PROPERTY(int toolbarHeight READ toolbarHeight NOTIFY toolbarHeightChanged)
    Q_PROPERTY(QString playlistHiglightTextColor READ playlistHiglightTextColor NOTIFY playlistHiglightTextColorChanged)

public:
    explicit ColorScheme(QObject *parent = nullptr);

    QString getColor(QString name, QString defaultValue = "white");

    QString textColor() { return getColor("text_color", "black"); }
    QString backgroundColor() { return getColor("background_color", "white"); }
    QString toolbarColor() { return getColor("toolbar_color", "black"); }
    QString menuColor() { return getColor("menu_color", "black"); }
    QString dividerColor() { return getColor("divider_color", "gray"); }
    QString primaryButtonColor() { return getColor("primary_button_color", "black"); }
    QString secondaryButtonColor() { return getColor("secondary_button_color", "darkgray"); }
    QString toolbarTextColor() { return getColor("toolbar_text_color", "white"); }
    QString buttonTextColor() { return getColor("button_text_color", "white"); }
    QString listHeaderBackgroundColor() { return getColor("list_header_background_color", "grey"); }
    QString listHeaderTextColor() { return getColor("list_header_text_color", "black"); }
    QString combatTrackerXType() { return getColor("combat_tracker_x_type", "dark"); }
    QString playlistHiglightTextColor() { return getColor("playlist_higlight_text_color", "black"); }
    int toolbarHeight() { return stylesheet->value("toolbar_height", 40).toInt(); }

signals:
    void textColorChanged();
    void backgroundColorChanged();
    void toolbarColorChanged();
    void menuColorChanged();
    void dividerColorChanged();
    void primaryButtonColorChanged();
    void secondaryButtonColorChanged();
    void toolbarTextColorChanged();
    void buttonTextColorChanged();
    void listHeaderBackgroundColorChanged();
    void listHeaderTextColorChanged();
    void combatTrackerXTypeChanged();
    void playlistHiglightTextColorChanged();
    void toolbarHeightChanged();

private:
    SettingsManager *sManager;
    QSettings *stylesheet;

    QString style;

};

#endif // COLORSCHEME_H
