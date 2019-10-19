#include "audioconverter.h"
#include "src/settings/settingsmanager.h"
#include "src/functions.h"

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

            if ((file_version == 0) && settings.value("ProjectName", "").toString().isEmpty())
            {
                QFile f(basePath + "/" + file);

                if (f.open(QIODevice::ReadOnly))
                {
                    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
                    f.close();
                    file_version = doc.object().value("version").toInt();
                }
            }

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

            if (file_version == 1)
            {
                file_version = 2;
                settings.setValue("version", 2);

                QStringList toRemove = { "_MusicList", "_MusicLists", "_Order", "_SoundLists", "_SoundList", "_Radios" };

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

            if (file_version == 2)
            {
                file_version = 3;
                convertTo3(basePath + "/" + file);
            }

            if (file_version == 3)
            {
                file_version = 4;
                convertTo4(basePath + "/" + file);
            }
            else
            {
                qDebug() << "   Project has already been converted.";
            }
        }
    }
}

void AudioConverter::convertTo3(QString file)
{
    qDebug() << "Converting project" << file << "to version 3 ...";
    QSettings settings(file, QSettings::IniFormat);

    if (settings.value("ProjectName").toString().isEmpty()) return;

    // Basic Info
    QJsonObject project;
    project.insert("name", settings.value("ProjectName").toString());
    project.insert("version",     3);

    // Categories
    QJsonArray  categories;
    QStringList categoryNames = settings.value("categories", {}).toStringList();

    qDebug() << categoryNames;

    for (auto c : categoryNames)
    {
        QJsonObject category;
        category.insert("name", c);

        // Scenarios
        settings.beginGroup(c);
        QStringList scenarioNames = settings.value("scenarios", {}).toStringList();
        QJsonArray  scenarios;
        settings.endGroup();

        for (auto s : scenarioNames)
        {
            QJsonObject scenario;
            scenario.insert("name", s);

            // Elements
            settings.beginGroup(c);
            QStringList musicElementNames   = settings.value(s + "_music").toStringList();
            QStringList soundElementNames   = settings.value(s + "_sounds").toStringList();
            QStringList radioElementNames   = settings.value(s + "_radios").toStringList();
            QStringList spotifyElementNames = settings.value(s + "_spotify").toStringList();
            settings.endGroup();

            QJsonArray musicElements;
            QJsonArray soundElements;
            QJsonArray radioElements;
            QJsonArray spotifyElements;

            // Music Lists
            for (auto e : musicElementNames)
            {
                QJsonObject element;
                settings.beginGroup(c + "_" + s + "_" + e + "_music");
                element.insert("name",        e);
                element.insert("icon", settings.value("icon").toString());
                element.insert("mode", settings.value("mode").toInt());

                QJsonArray files;
                int size = settings.beginReadArray("songs");

                for (int i = 0; i < size; i++)
                {
                    settings.setArrayIndex(i);
                    QStringList temp = settings.value("song").toStringList();

                    if (temp.size() > 1) files.append(temp[1]);
                    else files.append(temp[0]);
                }
                settings.endArray();
                settings.endGroup();

                element.insert("files", files);
                musicElements.append(element);
            }

            // Sound Lists
            for (auto e : soundElementNames)
            {
                QJsonObject element;
                settings.beginGroup(c + "_" + s + "_" + e + "_sounds");
                element.insert("name",        e);
                element.insert("icon", settings.value("icon").toString());
                element.insert("mode", settings.value("mode").toInt());

                QJsonArray files;
                int size = settings.beginReadArray("sounds");

                for (int i = 0; i < size; i++)
                {
                    settings.setArrayIndex(i);
                    QStringList temp = settings.value("sound").toStringList();

                    if (temp.size() > 1) files.append(temp[1]);
                    else files.append(temp[0]);
                }
                settings.endArray();
                settings.endGroup();

                element.insert("files", files);
                soundElements.append(element);
            }

            // Radios
            for (auto e : radioElementNames)
            {
                QJsonObject element;
                settings.beginGroup(c + "_" + s + "_" + e + "_radio");
                element.insert("name",         e);
                element.insert("icon",  settings.value("icon").toString());
                element.insert("url",   settings.value("url").toString());
                element.insert("local", settings.value("local", false).toBool());
                settings.endGroup();
                radioElements.append(element);
            }

            // Spotify
            for (auto e : spotifyElementNames)
            {
                QJsonObject element;
                settings.beginGroup(c + "_" + s + "_" + e + "_spotify");
                element.insert("name",        e);
                element.insert("icon", settings.value("icon").toString());
                element.insert("id",   settings.value("id").toString());
                settings.endGroup();
                spotifyElements.append(element);
            }

            scenario.insert("music_elements",     musicElements);
            scenario.insert("sound_elements",     soundElements);
            scenario.insert("radio_elements",     radioElements);
            scenario.insert("spotify_elements", spotifyElements);
            scenarios.append(scenario);
        }

        category.insert("scenarios", scenarios);
        categories.append(category);
    }

    project.insert("categories", categories);

    QJsonDocument doc(project);

    QFile(file).rename(file + "_old");
    QFile f(file);

    if (f.open(QIODevice::WriteOnly))
    {
        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();

        qDebug() << "Successfully converted" << file << "to version 3.";
    }
    else
    {
        qDebug() << "Error: Could not open file" << file << "!";
    }
}

void AudioConverter::convertTo4(QString file)
{
    qDebug() << "Converting project" << file << "to version 4 ...";

    QFile f(file);
    QJsonDocument doc;

    if (f.open(QIODevice::ReadOnly))
    {
        doc = QJsonDocument::fromJson(f.readAll());
        f.close();
    }
    else
    {
        qDebug() << "Error: Could not open file" << file << "!";
        return;
    }

    QJsonArray categories = doc.object().value("categories").toArray();
    QJsonArray categoriesNew;

    for (auto category : categories)
    {
        QJsonObject categoryNew{ { "name", category.toObject().value("name") } };

        QJsonArray scenariosNew;

        for (auto scenario : category.toObject().value("scenarios").toArray())
        {
            scenariosNew.append(convertScenarioTo4(scenario.toObject()));
        }

        categoryNew.insert("scenarios", scenariosNew);
        categoriesNew.append(categoryNew);
    }

    QJsonObject projectNew
    {
        { "name", doc.object().value("name") },
        { "version", 4 },
        { "categories", categoriesNew }
    };

    QJsonDocument docNew(projectNew);

    QFile(file).rename(file + "_old");
    QFile f2(file);

    if (f2.open(QIODevice::WriteOnly))
    {
        f2.write(docNew.toJson(QJsonDocument::Indented));
        f2.close();

        qDebug() << "Successfully converted" << file << "to version 4.";
    }
    else
    {
        qDebug() << "Error: Could not open file" << file << "!";
    }
}

QJsonObject AudioConverter::convertScenarioTo4(QJsonObject scenario)
{
    QJsonObject scenarioNew { { "name", scenario.value("name") } };

    // Music
    QJsonArray musicElementsNew;

    for (auto musicElement : scenario.value("music_elements").toArray())
    {
        QJsonObject musicElementNew
        {
            { "name", musicElement.toObject().value("name") },
            { "icon", musicElement.toObject().value("icon") },
            { "mode", musicElement.toObject().value("mode") }
        };

        QJsonArray filesNew;

        for (auto file : musicElement.toObject().value("files").toArray())
        {
            filesNew.append(QJsonObject
            {
                { "url", file.toString() },
                { "source", 0 }
            });
        }

        musicElementNew.insert("files", filesNew);
        musicElementsNew.append(musicElementNew);
    }

    for (auto spotifyElement : scenario.value("spotify_elements").toArray())
    {
        QJsonObject musicElementNew
        {
            { "name", spotifyElement.toObject().value("name") },
            { "icon", spotifyElement.toObject().value("icon") },
        };

        QJsonArray filesNew;
        filesNew.append(QJsonObject
        {
            { "url", spotifyElement.toObject().value("id") },
            { "source", 2 }
        });

        musicElementNew.insert("files", filesNew);
        musicElementsNew.append(musicElementNew);
    }
    scenarioNew.insert("music_elements", musicElementsNew);

    // Sound
    QJsonArray soundElementsNew;

    for (auto soundElement : scenario.value("sound_elements").toArray())
    {
        QJsonObject soundElementNew
        {
            { "name", soundElement.toObject().value("name") },
            { "icon", soundElement.toObject().value("icon") },
            { "mode", soundElement.toObject().value("mode") }
        };

        QJsonArray filesNew;

        for (auto file : soundElement.toObject().value("files").toArray())
        {
            filesNew.append(QJsonObject
            {
                { "url", file.toString() },
                { "source", 0 }
            });
        }

        soundElementNew.insert("files", filesNew);
        soundElementsNew.append(soundElementNew);
    }
    scenarioNew.insert("sound_elements", soundElementsNew);

    // Radio
    QJsonArray radioElementsNew;

    for (auto radioElement : scenario.value("radio_elements").toArray())
    {
        QJsonObject radioElementNew
        {
            { "name", radioElement.toObject().value("name") },
            { "icon", radioElement.toObject().value("icon") },
            { "mode", 0 }
        };

        QJsonArray filesNew;
        filesNew.append(QJsonObject
        {
            { "url", radioElement.toObject().value("url") },
            { "source", radioElement.toObject().value("local").toBool() ? 0 : 1 }
        });

        radioElementNew.insert("files", filesNew);
        radioElementsNew.append(radioElementNew);
    }
    scenarioNew.insert("radio_elements", radioElementsNew);

    // Subscenarios
    QJsonArray subscenariosNew;

    for (auto subscenario : scenario.value("scenarios").toArray())
    {
        subscenariosNew.append(convertScenarioTo4(subscenario.toObject()));
    }

    scenarioNew.insert("scenarios", subscenariosNew);

    return scenarioNew;
}
