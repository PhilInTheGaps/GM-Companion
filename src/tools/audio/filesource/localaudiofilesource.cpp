#include "localaudiofilesource.h"
#include "functions.h"

LocalAudioFileSource::LocalAudioFileSource(AudioSaveLoad *saveLoad, QObject *parent)
    : AudioFileSource(saveLoad, parent)
{
}

int LocalAudioFileSource::findProjects(bool editor)
{
    QList<AudioProject *> projects;

    for (QString s : getFiles(SettingsManager::getPath("audioPath")))
    {
        if (s.endsWith(".audio"))
        {
            auto project = m_audioSaveLoad->initProject(SettingsManager::getPath("audioPath") + "/" + s);

            if (project) projects.append(project);
        }
    }

    emit foundProjects(projects, editor);

    m_audioSaveLoad->findIconPaths(projects, editor);

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
            iconMap[icon] = SettingsManager::getPath("resourcesPath") + icon;
        }
    }

    emit foundIconPaths(iconMap, editor);

    return m_requestCount++;
}

void LocalAudioFileSource::deleteProject(AudioProject *project)
{
    if (!project) return;

    QFile f(SettingsManager::getPath("audioPath") + "/" + project->name() + ".audio");

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
