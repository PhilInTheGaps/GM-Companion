#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

//#include <QWidget>
#include <QStringList>
#include <QFrame>
#include <QSettings>
#include <QSignalMapper>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QKeyEvent>
#include <QList>

#include "gm-companion/settings/settingsmanager.h"

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
    QString currentCategory();
    Q_INVOKABLE void setCurrentCategory(QString category);

    // Scenarios
    QStringList scenarios();
    QString currentScenario();
    Q_INVOKABLE void setCurrentScenario(QString scenario);

    // Elements
    Q_INVOKABLE void findElements();
    QStringList elements();
    QString currentElement();
    Q_INVOKABLE QString elementIcon(QString element);
    Q_INVOKABLE void setCurrentElement(QString element);
    Q_INVOKABLE int elementType(int index);

    // Music
    Q_INVOKABLE void playMusic(QString element);
    Q_INVOKABLE void musicNext();
    Q_INVOKABLE void musicAgain();
    Q_INVOKABLE void musicPausePlay();
    Q_INVOKABLE void setMusicIndex(int index);
    QStringList songs();
    int currentSongIndex();
    bool isPlaying();
    Q_INVOKABLE QString getSongName();
    Q_INVOKABLE QString getArtist();
    Q_INVOKABLE QString getAlbum();

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

private slots:
    void onCurrentSongChanged();
    void onMetaDataChanged();

private:
    SettingsManager *sManager;

    // Music
    int musicVolume;
    QMediaPlayer *musicPlayer;
    QMediaPlaylist *musicPlaylist;
    QStringList l_songs;
    bool musicNotRadio;
    void getMetaData();
    void getMetaDataTagLib();

    QString l_songName;
    QString l_artist;
    QString l_album;

    // Project
    QStringList projects;
    QString l_currentProject;

    QStringList l_categories;
    QString l_currentCategory;

    QStringList l_scenarios;
    QString l_currentScenario;

    QStringList l_elements;
    QStringList l_elementIcons;
    QList<int> l_elementTypes;
    QString l_currentElement;

    bool l_isPlaying;

    int soundVolume;
    QList<QMediaPlayer*> soundPlayerList;

    QMediaPlaylist *radioPlaylist;
};

#endif // AUDIOTOOL_H
