#ifndef LOCALAUDIOFILESOURCE_H
#define LOCALAUDIOFILESOURCE_H

#include "audiofilesource.h"

class LocalAudioFileSource : public AudioFileSource
{
    Q_OBJECT

public:
    LocalAudioFileSource(AudioSaveLoad *saveLoad, QObject *parent = nullptr);

    int findProjects(bool editor = false) override;
    int findIconPaths(QStringList icons) override;
};

#endif // LOCALAUDIOFILESOURCE_H
