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
    ~SettingsTool();

    int currentLanguageIndex();
    Q_INVOKABLE QString getPath(QString type);
    Q_INVOKABLE void setPath(QString type, QString path);
    Q_INVOKABLE void setLanguage(QString language);

    int uiStyleIndex();
    Q_INVOKABLE void setUiStyle(QString style);

    Q_INVOKABLE void setAddonEnabled(QString addon, bool enabled);

    Q_INVOKABLE void setCheckForUpdates(bool check);
    Q_INVOKABLE bool getCheckForUpdates();

    Q_INVOKABLE void setServiceConnection(QString type) { SettingsManager::setSetting("serviceConnection", type); }
    Q_INVOKABLE QString getServiceConnection() const { return SettingsManager::getSetting("serviceConnection"); }
    Q_INVOKABLE void setServer(QString url) { SettingsManager::setServerUrl(url); }
    Q_INVOKABLE QString getServer() const { return SettingsManager::getServerUrl(); }

    Q_INVOKABLE void setSpotifyID(QString id) { SettingsManager::setSetting("spotifyID", id, "Spotify"); }
    Q_INVOKABLE QString getSpotifyID() const { return SettingsManager::getSetting("spotifyID", "", "Spotify"); }
    Q_INVOKABLE void setSpotifySecret(QString secret) { SettingsManager::setSetting("spotifySecret", secret, "Spotify"); }
    Q_INVOKABLE QString getSpotifySecret() const { return SettingsManager::getSetting("spotifySecret", "", "Spotify"); }
    Q_INVOKABLE void setSpotifyUsername(QString username) { SettingsManager::setSetting("spotifyUsername", username, "Spotify"); }
    Q_INVOKABLE QString getSpotifyUsername() { return SettingsManager::getSetting("spotifyUsername", "", "Spotify"); }
    Q_INVOKABLE void setSpotifyPassword(QString password) { SettingsManager::setPassword(SettingsManager::getSetting("spotifyUsername", "", "Spotify"), password, "Spotify"); }
    Q_INVOKABLE QString getSpotifyPassword() { return SettingsManager::getPassword(SettingsManager::getSetting("spotifyUsername", "", "Spotify"), "Spotify"); }

    Q_INVOKABLE QString getGoogleID() const { return SettingsManager::getSetting("googleID", "", "Google"); }
    Q_INVOKABLE QString getGoogleSecret() const { return SettingsManager::getSetting("googleSecret", "", "Google"); }
    Q_INVOKABLE void setGoogleConnect(bool connect) { SettingsManager::setSetting("googleConnect", connect, "Google"); }
    Q_INVOKABLE bool getGoogleConnect() { return SettingsManager::getSetting("googleConnect", "", "Google").toInt(); }
    Q_INVOKABLE void setGooglePath(QString name, QString value) { SettingsManager::setSetting(name, value, "Google"); }
    Q_INVOKABLE QString getGooglePath(QString name) { return SettingsManager::getSetting(name, "", "Google"); }

    Q_INVOKABLE void setCloudMode(int mode) { SettingsManager::setSetting("cloudMode", mode); }
    Q_INVOKABLE int getCloudMode() { return SettingsManager::getSetting("cloudMode").toInt(); }

    bool classicIcons() const { return SettingsManager::getSetting("classicIcons").toInt(); }
    void setClassicIcons(bool b) { SettingsManager::setSetting("classicIcons", b); emit classicIconsChanged(); }

    bool showToolNames() const { return SettingsManager::getSetting("showToolNames").toInt(); }
    void setShowToolNames(bool b) { SettingsManager::setSetting("showToolNames", b); emit showToolNamesChanged(); }

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
    QStringList languages = {"English", "Deutsch"};
    QStringList styles = {"Dark", "Bright"};

};

#endif // SETTINGSTOOL_H
