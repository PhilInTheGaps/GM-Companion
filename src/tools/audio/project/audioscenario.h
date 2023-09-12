#pragma once

#include "audioelement.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QtQml/qqmlregistration.h>

class AudioCategory;

class AudioScenario : public TreeItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit AudioScenario(const QString &name, const QString &path, const QList<AudioElement *> &elements,
                           const QList<AudioScenario *> &scenarios, AudioCategory *parent);
    explicit AudioScenario(const QString &name, const QString &path, const QList<AudioElement *> &elements,
                           const QList<AudioScenario *> &scenarios, AudioScenario *parent);
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

    Q_PROPERTY(QQmlListProperty<AudioElement> elements READ elementsQml NOTIFY elementsChanged)
    [[nodiscard]] auto elements(bool recursive = false) const -> QList<AudioElement *>;
    [[nodiscard]] auto elementsQml() -> QQmlListProperty<AudioElement>;

    auto element(const QString &name, AudioElement::Type type) -> AudioElement *;
    auto moveElement(AudioElement *element, int steps) -> bool;
    auto addElement(AudioElement *element) -> bool;
    auto removeElement(AudioElement *element, bool deleteElement = true) -> bool;
    void sortElements(bool recursive);

    auto addScenario(AudioScenario *scenario) -> bool;
    auto deleteScenario(AudioScenario *scenario) -> bool;
    auto moveScenario(AudioScenario *scenario, int steps) -> bool;
    [[nodiscard]] auto containsScenario(const QString &name) const -> bool;

    static void setFilterString(const QString &filter)
    {
        filterString = filter;
    }
    void refreshElements();

    READ_LIST_PROPERTY(AudioScenario, scenarios)
    READ_LIST_PROPERTY(AudioScenario, model)
    AUTO_PROPERTY_VAL2(bool, isSubscenario, false)

signals:
    void pathChanged();
    void elementsChanged();

private slots:
    void onScenariosChanged();

private:
    explicit AudioScenario(const QString &name, const QString &path, const QList<AudioElement *> &elements,
                           const QList<AudioScenario *> &scenarios, bool isSubscenario, QObject *parent);
    explicit AudioScenario(const QJsonObject &object, const QString &path, bool isSubscenario, QObject *parent);

    inline static QString filterString = QLatin1String();

    QString m_path;

    QList<AudioElement *> m_elements;

    void prepareElement(AudioElement *element);
    void releaseElement(AudioElement *element) const;
    void prepareScenario(AudioScenario *scenario);
    void updateModel();
};
