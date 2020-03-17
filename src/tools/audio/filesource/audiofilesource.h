#ifndef AUDIOFILESOURCE_H
#define AUDIOFILESOURCE_H

#include <QObject>
#include "settings/settingsmanager.h"
#include "tools/audio/audiosaveload.h"

class AudioFileSource : public QObject
{
    Q_OBJECT
public:
    explicit AudioFileSource(AudioSaveLoad *saveLoad, QObject *parent = nullptr) : QObject(parent) { }

    virtual int findProjects(bool editor = false) { return 0; }
    virtual int findIconPaths(QStringList icons, bool editor) { return 0; }

signals:
    void foundProjects(QList<AudioProject*> projects, bool forEditor);
    void foundIconPaths(QMap<QString, QString> iconMap, bool editor);

protected:
    SettingsManager sManager;
    AudioSaveLoad *audioSaveLoad;

    int m_requestCount = 0;
};

#endif // AUDIOFILESOURCE_H
