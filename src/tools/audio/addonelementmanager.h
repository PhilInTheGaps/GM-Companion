#ifndef ADDONELEMENTMANAGER_H
#define ADDONELEMENTMANAGER_H

#include <QObject>
#include "src/settings/settingsmanager.h"

class AddonElementManager : public QObject
{
    Q_OBJECT
public:
    explicit AddonElementManager(QObject *parent = nullptr);

    Q_INVOKABLE QStringList getSpotifyFolders() const { return m_spotifyFolders; }
    Q_INVOKABLE void setFolder(QString folder);
    Q_INVOKABLE QStringList getElements() const { return m_spotifyPlaylistNames; }
    Q_INVOKABLE void setAddElement(int index, bool add) { m_addSpotifyPlaylists[index] = add; }

    Q_INVOKABLE QStringList getURIs() const { return m_spotifyPlaylistURIs; }
    Q_INVOKABLE QList<bool> getChecked() const { return m_addSpotifyPlaylists; }
    Q_INVOKABLE void resetChecked();

private:
    SettingsManager sManager;

    void findAddons();
    QStringList m_addonPaths;
    QStringList m_spotifyFiles;

    void findSpotifyFolders();
    QStringList m_spotifyFolders;
    QString m_currentSpotifyFolder;

    void findSpotifyPlaylists();
    QStringList m_spotifyPlaylistNames;
    QStringList m_spotifyPlaylistURIs;
    QList<bool> m_addSpotifyPlaylists;

signals:
    void elementsChanged();
};

#endif // ADDONELEMENTMANAGER_H
