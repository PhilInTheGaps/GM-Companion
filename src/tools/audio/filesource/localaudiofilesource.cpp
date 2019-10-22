#include "localaudiofilesource.h"
#include "src/functions.h"

LocalAudioFileSource::LocalAudioFileSource(AudioSaveLoad *saveLoad, QObject *parent)
    : AudioFileSource(saveLoad, parent)
{
    audioSaveLoad = saveLoad;
}

int LocalAudioFileSource::findProjects(bool editor)
{
    QList<AudioProject *> projects;

    for (QString s : getFiles(sManager.getSetting(Setting::audioPath)))
    {
        if (s.endsWith(".audio"))
        {
            auto project = audioSaveLoad->initProject(sManager.getSetting(Setting::audioPath) + "/" + s);

            if (project) projects.append(project);
        }
    }

    emit foundProjects(projects, editor);

    audioSaveLoad->findIconPaths(projects, editor);

    return m_requestCount++;
}

int LocalAudioFileSource::findIconPaths(QStringList icons, bool editor)
{
    QMap<QString, QString> iconMap;

    for (auto icon : icons)
    {
        if (icon.startsWith("http:") || icon.startsWith("https:"))
        {
            iconMap[icon] = icon;
        }
        else if (!icon.isEmpty())
        {
            iconMap[icon] = sManager.getSetting(Setting::resourcesPath) + icon;
        }
    }

    emit foundIconPaths(iconMap, editor);

    return m_requestCount++;
}
