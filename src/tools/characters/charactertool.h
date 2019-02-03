#ifndef CHARACTERTOOL_H
#define CHARACTERTOOL_H

#include <QObject>
#include <QList>
#include "character.h"
#include "src/settings/settingsmanager.h"
#include "src/managers/filemanager.h"

class CharacterTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList characters READ characters NOTIFY charactersChanged)
    Q_PROPERTY(QStringList images READ images NOTIFY currentCharacterChanged)
    Q_PROPERTY(QString image READ image NOTIFY imageChanged)
    Q_PROPERTY(int imageIndex READ imageIndex NOTIFY imageChanged)
    Q_PROPERTY(bool active READ active NOTIFY charactersChanged)

public:
    explicit CharacterTool(FileManager *fManager, QObject *parent = nullptr);

    QStringList characters() const { return m_characterNames; }
    QStringList images() const { return m_images; }

    QString image() const { return m_image; }
    Q_INVOKABLE void setImage(int index);

    int imageIndex() const { return m_imageIndex; }
    Q_INVOKABLE void nextImage(bool right);

    bool active() const { return m_active; }
    Q_INVOKABLE void toggleCharacterActive(int index);

    Q_INVOKABLE void setCurrentCharacter(int index);
    Q_INVOKABLE void displayActiveCharacters(bool active);

signals:
    void charactersChanged();
    void currentCharacterChanged();
    void imageChanged();

private:
    SettingsManager sManager;
    FileManager *fileManager = nullptr;

    QStringList m_characterNames, m_images, m_inactiveCharacters;
    QString m_image;
    int m_imageIndex = 0;

    QList<Character*> m_characters;
    QList<Character*> m_visibleCharacters;
    Character *m_currentCharacter = nullptr;

    bool m_active = true;

    void updateImages();
    void loadInactiveCharacters();
    void saveInactiveCharacters();

private slots:
    void receivedCharacters(QList<Character*> characters);
    void receivedFiles(int reqId, QList<CharacterFile> files);
};


#endif // CHARACTERTOOL_H

