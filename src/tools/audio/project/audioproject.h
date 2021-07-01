#ifndef AUDIOPROJECT_H
#define AUDIOPROJECT_H

#include "audiocategory.h"
#include "utils/utils.h"

class AudioProject : public TreeItem
{
    Q_OBJECT
    Q_PROPERTY(QString oldName READ oldName WRITE setOldName NOTIFY oldNameChanged)
    Q_PROPERTY(bool wasRenamed READ wasRenamed WRITE setWasRenamed NOTIFY wasRenamedChanged)
    Q_PROPERTY(int version READ version CONSTANT)
    Q_PROPERTY(bool isSaved READ isSaved WRITE setIsSaved NOTIFY isSavedChanged)
    Q_PROPERTY(QObject* currentCategory READ currentCategory NOTIFY currentCategoryChanged)
    Q_PROPERTY(QList<QObject*> categories READ categoryObjects NOTIFY categoriesChanged)
    Q_PROPERTY(int categoryIndex READ categoryIndex NOTIFY currentCategoryChanged)
    Q_PROPERTY(QObject* currentScenario READ currentScenario NOTIFY currentScenarioChanged)

public:
    AudioProject(const QString& name, int version, QList<AudioCategory*> categories, QObject *parent = nullptr);
    AudioProject(QJsonObject object, QObject *parent = nullptr);

    QJsonObject toJson() const;

    QString oldName() const { return m_oldName; }
    void setOldName(const QString &name) { m_oldName = name; emit oldNameChanged(); }

    bool wasRenamed() const { return m_wasRenamed; }
    void setWasRenamed(bool renamed) { m_wasRenamed = renamed; emit wasRenamedChanged(); }

    int version() const { return m_version; }
    bool isSaved() const { return m_isSaved; }
    void setIsSaved(bool saved) { m_isSaved = saved; emit isSavedChanged(); }

    bool isCheckable() const override { return true; }

    QList<AudioCategory*> categories() const { return m_categories; }
    QList<QObject*> categoryObjects() const { return Utils::toQObjectList<AudioCategory*>(categories()); }
    QStringList categoryNames() const;
    int categoryIndex() const { return m_categories.indexOf(m_currentCategory); }
    bool deleteCategory(AudioCategory *category);
    bool addCategory(AudioCategory* category, bool setAsCurrent = false);

    AudioCategory* currentCategory() const { return m_currentCategory; }
    Q_INVOKABLE bool setCurrentCategory(AudioCategory *category);

    AudioScenario* currentScenario() const;
    QList<AudioElement *> elements() const;

signals:
    void oldNameChanged();
    void wasRenamedChanged();
    void isSavedChanged();
    void categoriesChanged();

    void currentCategoryChanged();
    void currentScenarioChanged();

private:
    QString m_oldName;
    int m_version;
    bool m_isSaved = true, m_wasRenamed = false;

    QList<AudioCategory*> m_categories;
    AudioCategory *m_currentCategory = nullptr;

    void connectSignals();
    void prepareCategory(AudioCategory *category);

private slots:
    void onWasEdited();
};

#endif // AUDIOPROJECT_H
