#ifndef CHARACTERSAVELOAD_H
#define CHARACTERSAVELOAD_H

#include "gm-companion/settings/settingsmanager.h"
#include <QObject>
#include <QSettings>

class CharacterSaveLoad : public QObject
{
    Q_OBJECT
public:
    explicit CharacterSaveLoad(QObject *parent = nullptr);

    Q_INVOKABLE void initializeSaving();
    Q_INVOKABLE void initializeLoading();

    Q_INVOKABLE void saveCharacter(QString character_name, QStringList table_names, QList<QVariant> table_values, QStringList text_groups, QStringList text_names, QStringList texts);
    Q_INVOKABLE void saveTable(QString character_name, QString name, QList<QVariant> values);
    Q_INVOKABLE void saveText(QString character_name, QString group, QString name, QString text);

    Q_INVOKABLE QStringList loadTable(QString character_name, QString name, int column);
    Q_INVOKABLE QString loadText(QString character_name, QString group, QString name);

signals:
    void saving();
    void loading();

private:
    SettingsManager *sManager;
};

#endif // CHARACTERSAVELOAD_H
