#include "localaudiofilesource.h"
#include "functions.h"

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

void LocalAudioFileSource::deleteProject(AudioProject *project)
{
    if (!project) return;

    QFile f(sManager.getSetting(Setting::audioPath) + "/" + project->name() + ".audio");

    qDebug() << "LocalAudioFileSource: Deleting project file" << f.fileName();

    if (f.exists())
    {
        if (!f.remove())
        {
            qWarning() << "Error: Could not remove project" << project->name();
        }
    }
    else
    {
        qWarning() << "Error: Project file" << f.fileName() << "does not exist.";
    }

    project->deleteLater();
}
