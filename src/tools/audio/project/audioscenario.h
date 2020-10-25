#ifndef AUDIOSCENARIO_H
#define AUDIOSCENARIO_H

#include "audioelement.h"
#include "../../../utils/utils.h"

class AudioScenario : public TreeItem
{
    Q_OBJECT
    Q_PROPERTY(bool isSubscenario READ isSubscenario NOTIFY isSubscenarioChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_PROPERTY(QList<QObject*> elements READ elementObjects NOTIFY elementsChanged)
    Q_PROPERTY(QList<QObject*> scenarios READ scenarioObjects NOTIFY scenariosChanged)
    Q_PROPERTY(QList<QObject*> model READ model NOTIFY scenariosChanged)

public:
    AudioScenario(const QString &name, const QString &path, QList<AudioElement*> musicLists,
                  QList<AudioElement*> soundLists, QList<AudioElement*> radios,
                  QList<AudioScenario*> scenarios, QObject *parent);
    AudioScenario(const QJsonObject &object, const QString &path, QObject *parent);

    QJsonObject toJson() const;

    bool isCheckable() const override { return true; }
    bool isSubscenario() const { return m_isSubscenario; }

    QString path() const override { return m_path; }

    AudioElement *element(const QString &name, const AudioElement::Type &type);
    QList<AudioElement*> elements(bool recursive = false) const;
    QList<AudioElement*> elements(const AudioElement::Type &type, bool recursive = false);
    QList<QObject*> elementObjects() const { return Utils::toQObjectList(elements(false)); }
    QStringList elementNames(const AudioElement::Type &type);
    bool moveElement(AudioElement *element, int steps);
    bool addElement(AudioElement *element);
    Q_INVOKABLE bool removeElement(AudioElement *element, bool deleteElement = true);
    void sortElements(bool recursive);
    void refreshElements();

    AudioScenario *scenario(const QString &name);
    QList<AudioScenario*> scenarios() const { return m_scenarios; }
    QList<QObject*> scenarioObjects() const { return Utils::toQObjectList(scenarios()); }
    QStringList scenarioNames() const;
    bool addScenario(AudioScenario *scenario);
    bool deleteScenario(AudioScenario *scenario);
    bool moveScenario(AudioScenario *scenario, int steps);
    AudioScenario *getScenario(AudioElement *element);
    QList<QObject*> model();

    static void setFilterString(const QString& filter) { filterString = filter; }

signals:
    void pathChanged();
    void elementsChanged();
    void scenariosChanged();
    void isSubscenarioChanged();
    void wasEdited();

private:
    static QString filterString;

    bool m_isSubscenario = false;
    QString m_path;

    QList<AudioElement*> m_musicLists;
    QList<AudioElement*> m_soundLists;
    QList<AudioElement*> m_radios;
    QList<AudioScenario*> m_scenarios;

    QList<AudioElement *> *getElementList(const AudioElement::Type &type);

    void prepareElement(AudioElement *element);
    void releaseElement(AudioElement *element);
    void prepareScenario(AudioScenario *scenario);
};

#endif
