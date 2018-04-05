#ifndef CHARACTERTOOL_H
#define CHARACTERTOOL_H

#include "gm-companion/settings/settingsmanager.h"
#include "gm-companion/functions.h"

#include <QObject>
#include <QList>
#include <QStringList>

class CharacterTool : public QObject
{
    Q_OBJECT
public:
    explicit CharacterTool(QObject *parent = nullptr);

    Q_INVOKABLE void addCharacter(QString template_name, QString character_name, QString player_name);
    Q_INVOKABLE void deleteCharacter(QString character_name);
    Q_INVOKABLE int getSheetIndex(QString template_name);
    Q_INVOKABLE QStringList getActiveCharacterList();
    Q_INVOKABLE QStringList getInactiveCharacterList();

    Q_INVOKABLE void setCharacterActive(QString character_name);
    Q_INVOKABLE void setCharacterInactive(QString character_name);

    Q_INVOKABLE QString getSheetTemplate(QString character_name);


signals:
    void charactersUpdated();

private:
    SettingsManager *sManager;
};

#endif // CHARACTERTOOL_H
