#include "audioexporter.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSettings>

AudioExporter::AudioExporter(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Audio Exporter ...";
}

// Get all categories
void AudioExporter::updateCategories()
{
    qDebug() << "AudioExporter: Updating Categories ...";

    m_categories.clear();
    emit categoriesChanged(); // To clear the qml list

    m_scenarios.clear();
    m_elements.clear();

    m_exportCategories.clear();
    m_exportScenarios.clear();
    m_exportElements.clear();

    if (m_project != "")
    {
        qDebug() << "AudioExporter: Finding Categories ...";

        QSettings settings(sManager.getSetting(Setting::audioPath) + "/" + m_project + ".audio", QSettings::IniFormat);
        m_categories = settings.value("categories").toStringList();

        qDebug() << "AudioExporter: Finding Scenarios ...";

        for (int i = 0; i < m_categories.size(); i++)
        {
            // Set category enabled
            m_exportCategories.insert(i, true);

            // Find scenarios in category and set them enabled
            QList<bool> l;          // Scenarios
            QList<QList<bool> > l2; // Elements

            settings.beginGroup(m_categories[i]);
            QStringList scenarios = settings.value("scenarios").toStringList();

            for (int j = 0; j < scenarios.size(); j++)
            {
                l.insert(j, true);

                // Find elements in scenario and set them enabled
                QStringList elements = settings.value(scenarios[j] + "_music").toStringList();
                elements.append(settings.value(scenarios[j] + "_sounds").toStringList());
                elements.append(settings.value(scenarios[j] + "_radios").toStringList());

                QList<bool> l3;

                for (int k = 0; k < elements.size(); k++)
                {
                    l3.insert(k, true);
                }

                l2.insert(j, l3);
            }

            settings.endGroup();

            m_exportScenarios.insert(i, l);
            m_exportElements.insert(i, l2);
        }
    }

    emit categoriesChanged();
    emit scenariosChanged();
    emit elementsChanged();
}

// Get all scenarios from current category
void AudioExporter::updateScenarios(bool enabled)
{
    m_scenarios.clear();
    m_elements.clear();

    if (enabled)
    {
        QSettings settings(sManager.getSetting(Setting::audioPath) + "/" + m_project + ".audio", QSettings::IniFormat);
        settings.beginGroup(m_category);

        m_scenarios = settings.value("scenarios").toStringList();

        settings.endGroup();
    }

    emit scenariosChanged();
    emit elementsChanged();
}

// Get all elements in current scenario
void AudioExporter::updateElements(bool enabled)
{
    m_elements.clear();

    if (enabled)
    {
        QSettings settings(sManager.getSetting(Setting::audioPath) + "/" + m_project + ".audio", QSettings::IniFormat);
        settings.beginGroup(m_category);

        m_elements.append(settings.value(m_scenario + "_music").toStringList());
        m_elements.append(settings.value(m_scenario + "_sounds").toStringList());
        m_elements.append(settings.value(m_scenario + "_radios").toStringList());

        settings.endGroup();
    }

    emit elementsChanged();
}

// Return the default export path
QString AudioExporter::getDefaultPath()
{
    return QDir::homePath() + "/.gm-companion/export";
}

// Export all files
void AudioExporter::exportFiles()
{
    QString path = m_path == "" ? getDefaultPath() : m_path;

    // Create path if it does not exist
    if (!QDir(path).exists())
    {
        QDir d = QDir(path);
        d.mkpath(path);
    }

    qDebug() << "AudioExporter: Exporting files to:" << path << "...";

    // Find all relevant elements
    QStringList elementsToExport;

    QSettings   settings(sManager.getSetting(Setting::audioPath) + "/" + m_project + ".audio", QSettings::IniFormat);
    QStringList categories = settings.value("categories").toStringList();

    for (int i = 0; i < categories.size(); i++)
    {
        if (isCategoryEnabled(i))
        {
            m_category = categories[i];
            settings.beginGroup(categories[i]);

            m_scenarios = settings.value("scenarios").toStringList();

            for (int j = 0; j < m_scenarios.size(); j++)
            {
                if (isScenarioEnabled(j))
                {
                    m_scenario = m_scenarios[j];

                    QStringList music  = settings.value(m_scenario + "_music").toStringList();
                    QStringList sounds = settings.value(m_scenario + "_sounds").toStringList();
                    QStringList radios = settings.value(m_scenario + "_radios").toStringList();

                    for (int k = 0; k < music.size(); k++)
                        if (isElementEnabled(k))
                        {
                            elementsToExport.append(m_category + "_" + m_scenario + "_" + music[k] + "_music");
                        }

                    for (int k = 0; k < sounds.size(); k++)
                        if (isElementEnabled(k + music.size()))
                        {
                            elementsToExport.append(m_category + "_" + m_scenario + "_" + sounds[k] + "_sounds");
                        }

                    for (int k = 0; k < radios.size(); k++)
                        if (isElementEnabled(k + music.size() + sounds.size()))
                        {
                            elementsToExport.append(m_category + "_" + m_scenario + "_" + radios[k] + "_radio");
                        }
                }
            }

            settings.endGroup();
        }
    }

    // Get all files that need to be copied
    QStringList files;
    QStringList filesTypes;

    for (int j = 0; j < elementsToExport.size(); j++)
    {
        QString e = elementsToExport[j];
        settings.beginGroup(e);

        QString array, item, type;

        if (e.endsWith("_music"))
        {
            array = "songs";
            item  = "song";
            type  = "music";
        }
        else if (e.endsWith("_sounds"))
        {
            array = "sounds";
            item  = "sound";
            type  = "sounds";
        }
        else if (e.endsWith("_radio"))
        {
            type = "radios";
        }

        // Music or Sounds
        if ((type == "music") || (type == "sounds"))
        {
            int size = settings.beginReadArray(array);

            for (int i = 0; i < size; i++)
            {
                settings.setArrayIndex(i);

                QStringList s = settings.value(item).toStringList();

                if (s.size() > 1)
                {
                    QString file = s[1];
                    files.append(file);
                    filesTypes.append(type);
                }
            }

            settings.endArray();
        }

        // Radios
        if (type == "radios")
        {
            qDebug() << "FOUND RADIO!";
            bool local = settings.value("local", false).toBool();

            if (local)
            {
                qDebug() << settings.value("url").toString();
                files.append(settings.value("url").toString());
                filesTypes.append(type);
            }
        }

        settings.endGroup();
    }

    qDebug() << "Copying ...";
    int index = 1;

    // Copy all the files
    for (int i = 0; i < files.size(); i++)
    {
        qDebug() << "   Progress:" << index << "/" << files.size();
        index++;

        QString file = files[i];
        QString type = filesTypes[i];
        QString origBasePath;

        if (type == "music") origBasePath = sManager.getSetting(Setting::musicPath);
        else if (type == "sounds") origBasePath = sManager.getSetting(Setting::soundPath);
        else if (type == "radios") origBasePath = sManager.getSetting(Setting::radioPath);
        else
        {
            qDebug() << "AudioExporter: ERROR: Found unknown file!";
        }

        QFile f(origBasePath + file);

        if (f.exists())
        {
            QString fileName = file.section("/", -1, -1);
            QString dir      = path + "/" + type + file.section("/", 0, -2);

            if (!QDir(dir).exists())
            {
                QDir d;
                d.mkpath(dir);
            }

            if (!QFile(dir + "/" + fileName).exists())
            {
                f.copy(dir + "/" + fileName);
            }
            else
            {
                qDebug() << "File already exists!";
            }
        }
        else
        {
            qDebug() << "File" << f.fileName() << "not found!";
        }
    }

    qDebug() << "Done ...";
}
