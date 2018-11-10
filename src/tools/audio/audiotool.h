#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

#include <QStringList>
#include <QSettings>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QKeyEvent>
#include <QList>

#include "src/settings/settingsmanager.h"
#include "spotify.h"

struct Song {
    QString title;
    QString path;
};

class AudioTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList projectList READ projectList NOTIFY projectListChanged)
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(QStringList scenarios READ scenarios NOTIFY scenariosChanged)
    Q_PROPERTY(QStringList elements READ elements NOTIFY elementsChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(QStringList songs READ songs NOTIFY songsChanged)
    Q_PROPERTY(int currentSongIndex READ currentSongIndex NOTIFY currentSongChanged)
    Q_PROPERTY(QStringList elementIcons READ elementIcons NOTIFY elementIconsChanged)

    Q_PROPERTY(QString currentProject READ currentProject WRITE setCurrentProject NOTIFY currentProjectChanged)
    Q_PROPERTY(QString currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)
    Q_PROPERTY(QString currentScenario READ currentScenario WRITE setCurrentScenario NOTIFY currentScenarioChanged)
    Q_PROPERTY(QString currentElement READ currentElement WRITE setCurrentElement NOTIFY currentElementChanged)


public:
    explicit AudioTool(QObject *parent = 0);

    // Project
    QStringList projectList();
    QString currentProject();
    void setCurrentProject(QString project);

    // Categories
    QStringList categories();
    QString currentCategory() const { return m_currentCategory; }
    Q_INVOKABLE void setCurrentCategory(QString category);

    // Scenarios
    QStringList scenarios();
    QString currentScenario() const { return m_currentScenario; }
    Q_INVOKABLE void setCurrentScenario(QString scenario);

    // Elements
    Q_INVOKABLE void findElements();
    QStringList elements() const { return m_elements; }
    QString currentElement() const { return m_currentElement; }
    QStringList elementIcons() const { return m_elementIcons; }
    Q_INVOKABLE QString elementIcon(QString element);
    Q_INVOKABLE void setCurrentElement(QString element);
    Q_INVOKABLE int elementType(int index) const { return m_elementTypes.at(index); }
    Q_INVOKABLE void clearElements() { m_elements.clear(); emit elementsChanged(); }

    // Music
    Q_INVOKABLE void playMusic(QString element);
    Q_INVOKABLE void musicNext();
    Q_INVOKABLE void musicAgain();
    Q_INVOKABLE void musicPausePlay();
    Q_INVOKABLE void setMusicIndex(int index);
    QStringList songs() const { return m_songs; }
    int currentSongIndex() const { return m_musicPlaylist->currentIndex(); }
    bool isPlaying() const { return m_isPlaying; }
    Q_INVOKABLE QString getSongName() const { return m_songName; }
    Q_INVOKABLE QString getArtist() const { return m_artist; }
    Q_INVOKABLE QString getAlbum() const { return m_album; }
    Q_INVOKABLE QString getCoverArt() const { return m_cover; }

    // Spotify
    Q_INVOKABLE void playSpotify(QString element);
    Q_INVOKABLE void grantSpotify() { m_spotify.grant(); }

    // Sound
    Q_INVOKABLE void playSound(QString element);
    Q_INVOKABLE bool isSoundPlayling(QString element);
    Q_INVOKABLE void removeSound(QString element);

    // Radio
    Q_INVOKABLE void playRadio(QString element);

    // Volume
    Q_INVOKABLE void setMusicVolume(float volume);
    Q_INVOKABLE void setSoundVolume(float volume);

signals:
    void projectListChanged();
    void currentProjectChanged();

    void categoriesChanged();
    void currentCategoryChanged();

    void scenariosChanged();
    void currentScenarioChanged();

    void elementsChanged();
    void currentElementChanged();

    void isPlayingChanged();
    void currentSongChanged();
    void songsChanged();
    void metaDataChanged();

    void elementIconsChanged();

    void soundPlayerRemoved(int index);

private slots:
    void onCurrentSongChanged() { if (m_musicNotRadio) currentSongChanged(); }
    void onMetaDataChanged();

    void onSpotifyIconChanged(int index, QString url);
    void onSoundPlaybackStateChanged(QMediaPlayer::State status);

private:
    SettingsManager *m_sManager;

    // Music
    int m_musicVolume;
    QMediaPlayer *m_musicPlayer;
    QMediaPlaylist *m_musicPlaylist;
    QStringList m_songs;
    bool m_musicNotRadio;
    void getMetaData();
    void getMetaDataTagLib();

    QString m_songName;
    QString m_artist;
    QString m_album;
    QString m_cover;
    QString convertCoverImage();

    // Sounds
    int m_soundVolume;
    QList<QMediaPlayer*> m_soundPlayerList;

    // Spotify
    Spotify m_spotify;
    int m_spotifyVolume;

    QMediaPlaylist *m_radioPlaylist;

    // Project
    QStringList m_projects;
    QString m_currentProject;

    QStringList m_categories;
    QString m_currentCategory;

    QStringList m_scenarios;
    QString m_currentScenario;

    QStringList m_elements;
    QStringList m_elementIcons;
    QList<int> m_elementTypes;
    QString m_currentElement;

    bool m_isPlaying = false;
    bool m_spotifyPlaying = false;
};

#endif // AUDIOTOOL_H
