#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include <QObject>
#include <QSettings>

class ColorScheme : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString textColor READ textColor NOTIFY colorsChanged)
    Q_PROPERTY(QString backgroundColor READ backgroundColor NOTIFY colorsChanged)
    Q_PROPERTY(QString secondaryBackgroundColor READ secondaryBackgroundColor NOTIFY colorsChanged)
    Q_PROPERTY(QString toolbarColor READ toolbarColor NOTIFY colorsChanged)
    Q_PROPERTY(QString menuColor READ menuColor NOTIFY colorsChanged)
    Q_PROPERTY(QString dividerColor READ dividerColor NOTIFY colorsChanged)
    Q_PROPERTY(QString primaryButtonColor READ primaryButtonColor NOTIFY colorsChanged)
    Q_PROPERTY(QString secondaryButtonColor READ secondaryButtonColor NOTIFY colorsChanged)
    Q_PROPERTY(QString toolbarTextColor READ toolbarTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QString buttonTextColor READ buttonTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QString listHeaderBackgroundColor READ listHeaderBackgroundColor NOTIFY colorsChanged)
    Q_PROPERTY(QString listHeaderTextColor READ listHeaderTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QString combatTrackerXType READ combatTrackerXType NOTIFY colorsChanged)
    Q_PROPERTY(int toolbarHeight READ toolbarHeight NOTIFY colorsChanged)
    Q_PROPERTY(QString playlistHiglightTextColor READ playlistHiglightTextColor NOTIFY colorsChanged)

    Q_PROPERTY(bool dark READ dark NOTIFY colorsChanged)

public:
    explicit ColorScheme(QObject *parent = nullptr);

    Q_INVOKABLE void updateColors();

    QString getColor(QString name, QString defaultValue = "white");
    QString textColor() { return getColor("text_color", "black"); }
    QString backgroundColor() { return getColor("background_color", "white"); }
    QString secondaryBackgroundColor() { return getColor("secondary_background_color", "grey"); }
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

    bool dark() const { return stylesheet->value("dark", true).toBool(); }

signals:
    void colorsChanged();


private:
    QSettings *stylesheet;

    QString style;

};

#endif // COLORSCHEME_H
