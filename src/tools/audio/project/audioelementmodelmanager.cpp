#include "audioelementmodelmanager.h"

#include <QQmlContext>
#include "tools/audio/audioicongenerator.h"
#include "logging.h"

AudioElementModelManager::AudioElementModelManager(QQmlApplicationEngine *qmlEngine,
    SoundPlayerController *soundController, QObject *parent)
    : QObject(parent), qmlEngine(qmlEngine), soundController(soundController)
{
    soundModel = new AudioElementModel(this);
    elementModel = new AudioElementModelModel(this);

    qmlEngine->rootContext()->setContextProperty("soundModel", soundModel);
    qmlEngine->rootContext()->setContextProperty("elementModel", elementModel);

    connect(soundController, &SoundPlayerController::soundsChanged,
            this, &AudioElementModelManager::onSoundsChanged);
}

/**
 * @brief Change the audio project
 */
void AudioElementModelManager::setProject(AudioProject *project)
{
    qCDebug(gmAudioElementModelManager()) << "Changing audio project ...";

    if (currentProject)
    {
        disconnect(currentProject, &AudioProject::currentScenarioChanged,
                   this, &AudioElementModelManager::onElementsChanged);
    }

    if (project)
    {
        connect(project, &AudioProject::currentScenarioChanged,
                this, &AudioElementModelManager::onElementsChanged);
    }

    currentProject = project;
    onElementsChanged();
}

void AudioElementModelManager::findElement(const QString &term)
{
    if (term.isEmpty())
    {

        return;
    }

    QList<QList<AudioElement *> > newElements;

    for (const auto &l : elementModel.)
    {
        QList<AudioElement *> l2;

        for (auto e : l)
        {
            if (e && e->name().contains(term, Qt::CaseInsensitive))
            {
                l2.append(e);
            }
        }

        newElements.append(l2);
    }

    for (int i = 0; i < newElements.length(); i++)
    {
        elementModel->element(i)->setElements(newElements[i]);
    }
}

/**
 * @brief When the audio elements change, update the model
 */
void AudioElementModelManager::onElementsChanged()
{
    qCDebug(gmAudioElementModelManager()) << "Updating element model ...";
    elementModel->clear();

    if (!currentProject) return;

    auto *currentScenario = currentProject->currentScenario();
    if (!currentScenario) return;

    auto scenarios = currentScenario->scenarios();
    scenarios.push_front(currentScenario);

    auto isFirst = true;

    for (auto *scenario : scenarios)
    {
        auto *model = new AudioElementModel(this);
        model->setName(isFirst ? "" : scenario->name());
        model->setElements(scenario->elements());

        isFirst = false;
        elementModel->append(model);
    }

    AudioIconGenerator::generateIcons(currentScenario);
}

/**
 * @brief Update the list of active songs when they change
 * @param elements List of active sound elements
 */
void AudioElementModelManager::onSoundsChanged(const QList<AudioElement *> &elements)
{
    soundModel->setElements(elements);
}
