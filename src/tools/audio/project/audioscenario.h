#ifndef AUDIOSCENARIO_H
#define AUDIOSCENARIO_H

#include "audioelement.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AudioCategory;

struct AudioScenarioElements
{
    AudioScenarioElements() = default;
    explicit AudioScenarioElements(QList<AudioElement *> music, QList<AudioElement *> sound,
                                   QList<AudioElement *> radios, QList<AudioScenario *> scenarios)
        : musicLists(std::move(music)), soundLists(std::move(sound)), radios(std::move(radios)),
          scenarios(std::move(scenarios))
    {
    }

    QList<AudioElement *> musicLists;
    QList<AudioElement *> soundLists;
    QList<AudioElement *> radios;
    QList<AudioScenario *> scenarios;
};

class AudioScenario : public TreeItem
{
    Q_OBJECT
public:
    explicit AudioScenario(const QString &name, const QString &path, const AudioScenarioElements &elements,
                           AudioCategory *parent);
    explicit AudioScenario(const QString &name, const QString &path, const AudioScenarioElements &elements,
                           AudioScenario *parent);
    explicit AudioScenario(const AudioScenario &other);
    explicit AudioScenario(const QJsonObject &object, const QString &path, AudioCategory *parent);
    explicit AudioScenario(const QJsonObject &object, const QString &path, AudioScenario *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;
    [[nodiscard]] auto isCheckable() const -> bool override
    {
        return true;
    }

    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    [[nodiscard]] auto path() const -> QString override
    {
        return m_path;
    }
    void setPath(const QString &path)
    {
        m_path = path;
        emit pathChanged();
    }

    Q_PROPERTY(QList<AudioElement *> elements READ elements NOTIFY elementsChanged)
    [[nodiscard]] auto elements(bool recursive = false) const -> QList<AudioElement *>;
    [[nodiscard]] auto elements(AudioElement::Type type, bool recursive = false) -> QList<AudioElement *>;

    auto element(const QString &name, AudioElement::Type type) -> AudioElement *;
    auto moveElement(AudioElement *element, int steps) -> bool;
    auto addElement(AudioElement *element) -> bool;
    auto removeElement(AudioElement *element, bool deleteElement = true) -> bool;
    void sortElements(bool recursive);

    Q_PROPERTY(QList<AudioScenario *> scenarios READ scenarios NOTIFY scenariosChanged)
    [[nodiscard]] auto scenarios() const -> QList<AudioScenario *>
    {
        return m_scenarios;
    }

    auto addScenario(AudioScenario *scenario) -> bool;
    auto deleteScenario(AudioScenario *scenario) -> bool;
    auto moveScenario(AudioScenario *scenario, int steps) -> bool;
    [[nodiscard]] auto containsScenario(const QString &name) const -> bool;

    Q_PROPERTY(QList<QObject *> model READ model NOTIFY scenariosChanged)
    auto model() -> QList<QObject *>;

    static void setFilterString(const QString &filter)
    {
        filterString = filter;
    }
    void refreshElements();

    AUTO_PROPERTY_VAL2(bool, isSubscenario, false)

signals:
    void pathChanged();
    void elementsChanged();
    void scenariosChanged();

private:
    explicit AudioScenario(const QString &name, const QString &path, const AudioScenarioElements &elements,
                           bool isSubscenario, QObject *parent);
    explicit AudioScenario(const QJsonObject &object, const QString &path, bool isSubscenario, QObject *parent);
    auto getElementList(AudioElement::Type type) -> QList<AudioElement *> &;

    inline static QString filterString = QLatin1String();

    QString m_path;

    QList<AudioElement *> m_musicLists;
    QList<AudioElement *> m_soundLists;
    QList<AudioElement *> m_radios;
    QList<AudioScenario *> m_scenarios;

    void prepareElement(AudioElement *element);
    void releaseElement(AudioElement *element) const;
    void prepareScenario(AudioScenario *scenario);
};

#endif
