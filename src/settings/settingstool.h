#ifndef SETTINGSTOOL_H
#define SETTINGSTOOL_H

#include "settingsmanager.h"

#include <QObject>
#include <QFile>

class SettingsTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentLanguageIndex READ currentLanguageIndex NOTIFY currentLanguageIndexChanged)
    Q_PROPERTY(int uiStyleIndex READ uiStyleIndex NOTIFY uiStyleChanged)
    Q_PROPERTY(bool showToolNames READ showToolNames WRITE setShowToolNames NOTIFY showToolNamesChanged)
    Q_PROPERTY(bool classicIcons READ classicIcons WRITE setClassicIcons NOTIFY classicIconsChanged)

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
    Q_INVOKABLE void setSpotifyUsername(QString username) { sManager->setSetting(Setting::spotifyUsername, 1, username); }
    Q_INVOKABLE QString getSpotifyUsername() { return sManager->getSetting(Setting::spotifyUsername); }
    Q_INVOKABLE void setSpotifyPassword(QString password) { sManager->setSetting(Setting::spotifyPassword, 1, password); }
    Q_INVOKABLE QString getSpotifyPassword() { return sManager->getSetting(Setting::spotifyPassword); }

    Q_INVOKABLE QString getGoogleID() const { return sManager->getSetting(Setting::googleID); }
    Q_INVOKABLE QString getGoogleSecret() const { return sManager->getSetting(Setting::googleSecret); }
    Q_INVOKABLE void setGoogleConnect(bool connect) { sManager->setSetting(Setting::googleConnect, connect); }
    Q_INVOKABLE bool getGoogleConnect() { return sManager->getSetting(Setting::googleConnect).toInt(); }
    Q_INVOKABLE void setGooglePath(QString name, QString value) { sManager->setSetting(Setting::googlePath, 1, name, value); }
    Q_INVOKABLE QString getGooglePath(QString name) { return sManager->getSetting(Setting::googlePath, name); }

    Q_INVOKABLE void setCloudMode(int mode) { sManager->setSetting(Setting::cloudMode, 1, QString::number(mode)); }
    Q_INVOKABLE int getCloudMode() { return sManager->getSetting(Setting::cloudMode).toInt(); }

    bool classicIcons() const { return sManager->getSetting(Setting::classicIcons).toInt(); }
    void setClassicIcons(bool b) { sManager->setSetting(Setting::classicIcons, b); emit classicIconsChanged(); }

    bool showToolNames() const { return sManager->getSetting(Setting::showToolNames).toInt(); }
    void setShowToolNames(bool b) { sManager->setSetting(Setting::showToolNames, b); emit showToolNamesChanged(); }

    Q_INVOKABLE QString getCreditsPage();

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
    void showToolNamesChanged();
    void classicIconsChanged();

private:
    SettingsManager *sManager;
    QStringList languages = {"English", "Deutsch"};
    QStringList styles = {"Dark", "Bright"};

};

#endif // SETTINGSTOOL_H
