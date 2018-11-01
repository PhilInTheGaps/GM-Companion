#ifndef SETTINGSTOOL_H
#define SETTINGSTOOL_H

#include "settingsmanager.h"

#include <QObject>

class SettingsTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentLanguageIndex READ currentLanguageIndex NOTIFY currentLanguageIndexChanged)
    Q_PROPERTY(int uiStyleIndex READ uiStyleIndex NOTIFY uiStyleChanged)

public:
    explicit SettingsTool(QObject *parent = nullptr);

    int currentLanguageIndex();
    Q_INVOKABLE QString getPath(QString type);
    Q_INVOKABLE void setPath(QString type, QString path);
    Q_INVOKABLE void setLanguage(QString lang);

    int uiStyleIndex();
    Q_INVOKABLE void setUiStyle(QString style);

    Q_INVOKABLE void setAddonEnabled(QString addon, bool enabled);

    Q_INVOKABLE void setCheckForUpdates(bool check);
    Q_INVOKABLE bool getCheckForUpdates();

    Q_INVOKABLE void setSpotifyID(QString id) { sManager->setSetting(Setting::spotifyID, 1, id); }
    Q_INVOKABLE QString getSpotifyID() const { return sManager->getSetting(Setting::spotifyID); }
    Q_INVOKABLE void setSpotifySecret(QString secret) { sManager->setSetting(Setting::spotifySecret, 1, secret); }
    Q_INVOKABLE QString getSpotifySecret() const { return sManager->getSetting(Setting::spotifySecret); }

signals:
    void audioPathChanged();
    void musicPathChanged();
    void soundPathChanged();
    void radioPathChanged();
    void mapsPathChanged();
    void shopPathChanged();
    void charactersPathChanged();
    void notesPathChanged();
    void resourcesPathChanged();

    void currentLanguageIndexChanged();
    void uiStyleChanged();
    void checkForUdatesChanged();

private:
    SettingsManager *sManager;
    QStringList languages = {"English", "Deutsch"};
    QStringList styles = {"Dark", "Bright"};

};

#endif // SETTINGSTOOL_H
