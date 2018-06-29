#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include <QObject>
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

public:
    explicit ColorScheme(QObject *parent = nullptr);

    QString textColor();
    QString backgroundColor();
    QString toolbarColor();
    QString menuColor();
    QString dividerColor();
    QString primaryButtonColor();
    QString secondaryButtonColor();
    QString toolbarTextColor();
    QString buttonTextColor();

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

private:
    SettingsManager *sManager;

    QString style;
};

#endif // COLORSCHEME_H
