#ifndef CHARACTERTOOL_H
#define CHARACTERTOOL_H

#include "src/settings/settingsmanager.h"

#include <QObject>
#include <QStringList>
#include <QSettings>

class CharacterTool : public QObject
{
    Q_OBJECT
public:
    explicit CharacterTool(QObject *parent = nullptr);

    Q_INVOKABLE void updateCharacterList();

    Q_INVOKABLE QStringList getActiveCharacterList() const { return m_activeCharacters; }
    Q_INVOKABLE QStringList getInactiveCharacterList() const { return m_inactiveCharacters; }

    Q_INVOKABLE QStringList getImages(QString character_name);

    Q_INVOKABLE void setCharacterActive(QString character_name);
    Q_INVOKABLE void setCharacterInactive(QString character_name);

    Q_INVOKABLE void setActive(bool active) { m_active = active; }
    Q_INVOKABLE bool getActive() const { return m_active; }

    Q_INVOKABLE QString getButtonText(int index) const { return m_buttonTexts.at(index); }
    Q_INVOKABLE void loadButtonTexts(QString character_name);

signals:
    void charactersUpdated();

private:
    SettingsManager m_sManager;
    QSettings *m_settings;

    QStringList m_activeCharacters;
    QStringList m_inactiveCharacters;
    QStringList m_buttonTexts;

    bool m_active = true;
};


#endif // CHARACTERTOOL_H

