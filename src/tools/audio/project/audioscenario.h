#ifndef AUDIOSCENARIO_H
#define AUDIOSCENARIO_H

#include "audioelement.h"
#include "../../../utils/utils.h"

class AudioScenario : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool isMarkedForExport READ isMarkedForExport WRITE setIsMarkedForExport NOTIFY isMarkedForExportChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_PROPERTY(QList<QObject*> elements READ elementObjects NOTIFY elementsChanged)
public:
    AudioScenario(const QString &name, const QString &path, QList<AudioElement*> musicLists,
                  QList<AudioElement*> soundLists, QList<AudioElement*> radios,
                  QList<AudioScenario*> scenarios, QObject *parent = nullptr);
    AudioScenario(const QJsonObject &object, const QString &path, QObject *parent = nullptr);

    QJsonObject toJson() const;

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }

    bool isMarkedForExport() const { return m_export; }
    void setIsMarkedForExport(bool e) { m_export = e; emit isMarkedForExportChanged(); }

    QString path() const { return m_path; }

    AudioElement *element(const QString &name, const AudioElement::Type &type);
    QList<AudioElement*> elements(bool recursive = false) const;
    QList<AudioElement*> elements(const AudioElement::Type &type, bool recursive = false);
    QList<QObject*> elementObjects() const { return Utils::toQObjectList(elements(false)); }
    QStringList elementNames(const AudioElement::Type &type);
    bool moveElement(const QString &name, int steps, const AudioElement::Type &type);
    bool addElement(AudioElement *element);
    bool removeElement(const QString &name, const AudioElement::Type &type, bool deleteElement = true);
    void sortElements();
    void refreshElements();

    AudioScenario *scenario(const QString &name);
    QList<AudioScenario*> scenarios() const { return m_scenarios; }
    QStringList scenarioNames() const;
    void addScenario(AudioScenario *scenario) { m_scenarios.append(scenario); }
    bool removeScenario(const QString &name);
    bool moveScenario(const QString &name, int steps);
    bool deleteScenario(const QString &name);

    static void setFilterString(const QString& filter) { filterString = filter; }

signals:
    void nameChanged();
    void isMarkedForExportChanged();
    void pathChanged();
    void elementsChanged();

private:
    static QString filterString;

    bool m_export = true;
    QString m_name;
    QString m_path;

    QList<AudioElement*> m_musicLists;
    QList<AudioElement*> m_soundLists;
    QList<AudioElement*> m_radios;
    QList<AudioScenario*> m_scenarios;

    QList<AudioElement*>& getElementList(const AudioElement::Type &type);
};

#endif
