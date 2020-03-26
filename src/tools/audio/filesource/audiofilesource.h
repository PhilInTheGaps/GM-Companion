#ifndef AUDIOFILESOURCE_H
#define AUDIOFILESOURCE_H

#include <QObject>
#include "tools/audio/audiosaveload.h"

class AudioFileSource : public QObject
{
    Q_OBJECT
public:
    explicit AudioFileSource(AudioSaveLoad *saveLoad, QObject *parent = nullptr) : QObject(parent), m_audioSaveLoad(saveLoad) { }

    virtual int findProjects(bool editor = false) = 0;
    virtual int findIconPaths(QStringList icons, bool editor) = 0;

signals:
    void foundProjects(QList<AudioProject*> projects, bool forEditor);
    void foundIconPaths(QMap<QString, QString> iconMap, bool editor);

protected:
    AudioSaveLoad *m_audioSaveLoad;

    int m_requestCount = 0;
};

#endif // AUDIOFILESOURCE_H
