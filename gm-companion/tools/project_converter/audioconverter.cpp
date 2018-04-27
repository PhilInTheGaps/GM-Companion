#include "audioconverter.h"
#include "gm-companion/settings/settingsmanager.h"
#include "gm-companion/functions.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

struct MusicList
{
    QString           name;
    QString           iconPath;
    int               mode;
    QList<QStringList>songs;
};

struct SoundList
{
    QString           name;
    QString           iconPath;
    int               mode;
    QList<QStringList>sounds;
};

struct Radio
{
    QString name;
    QString iconPath;
    QString url;
    bool    local;
};

AudioConverter::AudioConverter(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Shop Converter ...";
}

void AudioConverter::convert()
{
    SettingsManager *sManager = new SettingsManager;

    qDebug() << "Converting Audio Projects ...";

    QString basePath = sManager->getSetting(Setting::audioPath);

    for (QString file : getFiles(basePath))
    {
        if (file.endsWith(".ini"))
        {
            QFile   f(basePath + "/" + file);
            QString newName = file.replace(".ini", ".audio");
            f.rename(basePath + "/" + newName);
        }

        if (file.endsWith(".audio"))
        {
            qDebug() << "   Converting Audio Project:" << file << "...";

            QSettings settings(basePath + "/" + file, QSettings::IniFormat);

            int file_version = settings.value("version", 0).toInt();

            if (file_version == 0)
            {
                settings.setValue("version", 1);
                file_version = 1;

                // Change the way categories are saved
                QStringList categories;

                int catCount = settings.beginReadArray("Categories");

                for (int i = 0; i < catCount; i++)
                {
                    settings.setArrayIndex(i);
                    categories.append(settings.value("name").toString());
                }

                settings.endArray();

                settings.setValue("categories", categories);

                // Scenarios
                for (int i = 0; i < catCount; i++)
                {
                    int scenCount = settings.beginReadArray(categories.at(i) + "_Scenarios");

                    QStringList scenarios;

                    for (int j = 0; j < scenCount; j++)
                    {
                        settings.setArrayIndex(j);
                        scenarios.append(settings.value("name").toString());
                    }

                    settings.endArray();


                    // Elements
                    QList<QList<MusicList> > musicListsList; // List of list of
                                                             // MusicLists for
                                                             // all scenarios
                    QList<QList<SoundList> > soundListsList;
                    QList<QList<Radio> > radioListsList;

                    for (int j = 0; j < scenCount; j++)
                    {
                        // Music Lists
                        QList<MusicList> musicLists;

                        int mListCount = settings.beginReadArray(categories.at(i) + "_" + scenarios.at(j) + "_MusicLists");

                        for (int k = 0; k < mListCount; k++)
                        {
                            settings.setArrayIndex(k);

                            MusicList list;
                            list.name     = settings.value("name").toString();
                            list.iconPath = settings.value("icon").toString();

                            int mode = 0;

                            if (settings.value("randomPlaylist", false).toBool()) mode = 0;

                            if (settings.value("randomPlayback", false).toBool()) mode = 1;

                            if (settings.value("loop", false).toBool()) mode = 2;

                            if (settings.value("sequential", false).toBool()) mode = 3;

                            list.mode = mode;

                            int songCount = settings.beginReadArray("songs");

                            QList<QStringList> songs;

                            for (int l = 0; l < songCount; l++)
                            {
                                settings.setArrayIndex(l);

                                QStringList song;
                                song.append(settings.value("name").toString());
                                song.append(settings.value("path").toString());

                                songs.append(song);
                            }

                            settings.endArray();

                            list.songs = songs;

                            musicLists.append(list);
                        }

                        settings.endArray();

                        musicListsList.append(musicLists);

                        // Sound Lists
                        QList<SoundList> soundLists;

                        int sListCount = settings.beginReadArray(categories.at(i) + "_" + scenarios.at(j) + "_SoundLists");

                        for (int k = 0; k < sListCount; k++)
                        {
                            settings.setArrayIndex(k);

                            SoundList list;
                            list.name     = settings.value("name").toString();
                            list.iconPath = settings.value("icon").toString();

                            int mode = 0;

                            if (settings.value("randomPlaylist", false).toBool()) mode = 0;

                            if (settings.value("random", false).toBool()) mode = 1;

                            if (settings.value("loop", false).toBool()) mode = 2;

                            if (settings.value("sequential", false).toBool()) mode = 3;

                            list.mode = mode;

                            int soundCount = settings.beginReadArray("sounds");

                            QList<QStringList> sounds;

                            for (int l = 0; l < soundCount; l++)
                            {
                                settings.setArrayIndex(l);

                                QStringList sound;
                                sound.append(settings.value("name").toString());
                                sound.append(settings.value("path").toString());

                                sounds.append(sound);
                            }

                            settings.endArray();

                            list.sounds = sounds;

                            soundLists.append(list);
                        }

                        settings.endArray();

                        soundListsList.append(soundLists);

                        // Radios
                        QList<Radio> radios;

                        int rCount = settings.beginReadArray(categories.at(i) + "_" + scenarios.at(j) + "_Radios");

                        for (int k = 0; k < rCount; k++)
                        {
                            settings.setArrayIndex(k);

                            Radio radio;
                            radio.name     = settings.value("name").toString();
                            radio.iconPath = settings.value("icon").toString();
                            radio.url      = settings.value("URL").toString();
                            radio.local    = radio.url.startsWith("http") ? false : true;

                            radios.append(radio);
                        }

                        settings.endArray();

                        radioListsList.append(radios);
                    }

                    settings.beginGroup(categories.at(i));
                    settings.setValue("scenarios", scenarios);

                    // Save list names
                    for (int k = 0; k < scenCount; k++)
                    {
                        // Music Lists
                        QStringList musicLists;

                        for (int l = 0; l < musicListsList.at(k).size(); l++)
                        {
                            musicLists.append(musicListsList.at(k).at(l).name);
                        }

                        settings.setValue(scenarios.at(k) + "_music", musicLists);

                        // Sound Lists
                        QStringList soundLists;

                        for (int l = 0; l < soundListsList.at(k).size(); l++)
                        {
                            soundLists.append(soundListsList.at(k).at(l).name);
                        }

                        settings.setValue(scenarios.at(k) + "_sounds", soundLists);

                        // Radios
                        QStringList radios;

                        for (int l = 0; l < radioListsList.at(k).size(); l++)
                        {
                            radios.append(radioListsList.at(k).at(l).name);
                        }

                        settings.setValue(scenarios.at(k) + "_radios", radios);
                    }

                    settings.endGroup();

                    // Save lists themselves
                    for (int k = 0; k < scenCount; k++)
                    {
                        // Music Lists
                        for (int l = 0; l < musicListsList.at(k).size(); l++)
                        {
                            MusicList list = musicListsList.at(k).at(l);
                            QString   name = list.name;
                            settings.beginGroup(categories.at(i) + "_" + scenarios.at(k) + "_" + name + "_music");

                            settings.setValue("name", name);
                            settings.setValue("icon", list.iconPath);
                            settings.setValue("mode", list.mode);

                            settings.beginWriteArray("songs");

                            for (int m = 0; m < list.songs.size(); m++)
                            {
                                settings.setArrayIndex(m);
                                settings.setValue("song", list.songs.at(m));
                            }

                            settings.endArray();

                            settings.endGroup();
                        }

                        // Sound Lists
                        for (int l = 0; l < soundListsList.at(k).size(); l++)
                        {
                            SoundList list = soundListsList.at(k).at(l);
                            QString   name = list.name;
                            settings.beginGroup(categories.at(i) + "_" + scenarios.at(k) + "_" + name + "_sounds");

                            settings.setValue("name", name);
                            settings.setValue("icon", list.iconPath);
                            settings.setValue("mode", list.mode);

                            settings.beginWriteArray("sounds");

                            for (int m = 0; m < list.sounds.size(); m++)
                            {
                                settings.setArrayIndex(m);
                                settings.setValue("sound", list.sounds.at(m));
                            }

                            settings.endArray();

                            settings.endGroup();
                        }

                        // Radios
                        for (int l = 0; l < radioListsList.at(k).size(); l++)
                        {
                            Radio   radio = radioListsList.at(k).at(l);
                            QString name  = radio.name;
                            settings.beginGroup(categories.at(i) + "_" + scenarios.at(k) + "_" + name + "_radio");

                            settings.setValue("name",   name);
                            settings.setValue("icon",  radio.iconPath);
                            settings.setValue("local", radio.local);
                            settings.setValue("url",   radio.url);

                            settings.endGroup();
                        }
                    }
                }
            }
            else if (file_version == 1)
            {
                file_version = 2;
                settings.setValue("version", 2);

                QStringList toRemove = {"_MusicList", "_MusicLists", "_Order", "_SoundLists", "_SoundList", "_Radios"};

                for (QString group : settings.childGroups())
                {
                    for (QString ending : toRemove)
                    {
                        if (group.endsWith(ending))
                        {
                            qDebug() << group;
                            settings.remove(group);
                        }
                    }
                }
            }
            else
            {
                qDebug() << "   Project has already been converted.";
            }
        }
    }
}
