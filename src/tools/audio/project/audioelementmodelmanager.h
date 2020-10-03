#ifndef AUDIOELEMENTMODELMANAGER_H
#define AUDIOELEMENTMODELMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "audioproject.h"
#include "audioelementmodel.h"
#include "../players/soundplayer.h"

class AudioElementModelManager : public QObject
{
    Q_OBJECT
public:
    explicit AudioElementModelManager(QQmlApplicationEngine *qmlEngine,
                                      SoundPlayerController *soundController,
                                      QObject *parent = nullptr);

    void setProject(AudioProject *project);
    void findElement(const QString& term);

private:
    QQmlApplicationEngine *qmlEngine = nullptr;
    AudioElementModelModel *elementModel = nullptr;
    AudioElementModel *soundModel = nullptr;

    AudioProject *currentProject = nullptr;
    SoundPlayerController *soundController = nullptr;

private slots:
    void onElementsChanged();
    void onSoundsChanged(const QList<AudioElement*>& elements);
};

#endif // AUDIOELEMENTMODELMANAGER_H
