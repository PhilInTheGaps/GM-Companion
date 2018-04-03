#ifndef CHARACTERSHEET_H
#define CHARACTERSHEET_H

#include <QObject>

class CharacterSheet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString template_name READ getTemplateName WRITE setTemplateName NOTIFY templateNameChanged)
    Q_PROPERTY(QString player_name READ getPlayerName WRITE setPlayerName NOTIFY playerNameChanged)
    Q_PROPERTY(QString character_name READ getCharacterName WRITE setCharacterName NOTIFY characterNameChanged)

public:
    explicit CharacterSheet(QObject *parent = nullptr);

    QString getTemplateName();
    void setTemplateName(QString name);

    QString getPlayerName();
    void setPlayerName(QString name);

    QString getCharacterName();
    void setCharacterName(QString name);

signals:
    void templateNameChanged();
    void playerNameChanged();
    void characterNameChanged();

protected:
    QString l_template_name;
    QString l_player_name;
    QString l_character_name;

};

#endif // CHARACTERSHEET_H
