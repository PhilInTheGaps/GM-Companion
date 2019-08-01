#include "charactertool.h"
#include "src/functions.h"

#include <QDebug>
#include <QFile>
#include <QSettings>

CharacterTool::CharacterTool(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent), fileManager(fManager)
{
    qDebug() << "Loading Character Tool ...";

    m_imageViewer = new CharacterImageViewer;
    m_dsa5Viewer  = new CharacterDSA5Viewer(engine);

    connect(m_imageViewer,                          &CharacterImageViewer::categoryChanged,    [ = ]() { if (m_currentViewer == m_imageViewer) emit categoryChanged(); });
    //connect(m_dsa5Viewer,                           &CharacterImageViewer::categoryChanged,    [ = ]() { if (m_currentViewer == m_dsa5Viewer) emit categoryChanged(); });

    connect(m_imageViewer,                          &CharacterImageViewer::categoriesChanged,  [ = ]() { if (m_currentViewer == m_imageViewer) emit categoriesChanged(); });
    //connect(m_dsa5Viewer,                           &CharacterDSA5Viewer::categoriesChanged,   [ = ]() { if (m_currentViewer == m_dsa5Viewer) emit categoriesChanged(); });

    connect(fileManager->getCharacterFileManager(), &CharacterFileManager::receivedCharacters, this, &CharacterTool::receivedCharacters);
    connect(fileManager->getCharacterFileManager(), &CharacterFileManager::receivedFiles,      this, &CharacterTool::receivedFiles);

    loadInactiveCharacters();
    fileManager->getCharacterFileManager()->findCharacters(fileManager->getModeInt());
}

void CharacterTool::receivedCharacters(QList<Character *>characters)
{
    qDebug() << "CharacterTool: Received characters ...";

    m_characters = characters;
    m_visibleCharacters.clear();
    m_characterNames.clear();

    for (auto c : characters)
    {
        if (c && !m_inactiveCharacters.contains(c->name()))
        {
            m_visibleCharacters.append(c);
            m_characterNames.append(c->name());
        }
    }

    emit charactersChanged();
}

void CharacterTool::updateCharacter()
{
    if (!m_currentCharacter || !m_currentViewer) return;

    if (m_currentCharacter->type() < 2)
    {
        m_imageViewer->updateImages();
        m_imageViewer->setCurrentCategory(0);
    }
}

void CharacterTool::receivedFiles(int reqId, QList<CharacterFile>files)
{
    if (m_currentCharacter && (m_currentCharacter->requestId() == reqId))
    {
        m_currentCharacter->setFiles(files);
        updateCharacter();
        emit currentCharacterChanged();
    }
    else
    {
        for (auto c : m_characters)
        {
            if (c && (c->requestId() == reqId))
            {
                c->setFiles(files);
                return;
            }
        }
    }
}

void CharacterTool::setCurrentCharacter(int index)
{
    if ((index > -1) && (index < m_visibleCharacters.size()))
    {
        m_currentCharacter = m_visibleCharacters[index];

        if (m_currentCharacter->type() < 2) m_currentViewer = m_imageViewer;
        else m_currentViewer = m_dsa5Viewer;

        m_currentViewer->setCharacter(m_currentCharacter);

        emit currentCharacterChanged();
        emit pageIndexChanged();

        if ((m_currentCharacter->files().size() == 0) && (m_currentCharacter->requestId() == -1))
        {
            fileManager->getCharacterFileManager()->findFiles(m_currentCharacter);
        }

        return;
    }

    m_currentCharacter = nullptr;
    emit currentCharacterChanged();
    emit pageIndexChanged();
}

void CharacterTool::setCurrentCategory(int index)
{
    if (!m_currentViewer) return;

    m_currentViewer->setCurrentCategory(index);
}

void CharacterTool::toggleCharacterActive(int index)
{
    if ((index > -1) && (index < m_visibleCharacters.size()))
    {
        if (m_active)
        {
            m_inactiveCharacters.append(m_visibleCharacters[index]->name());
        }
        else
        {
            m_inactiveCharacters.removeOne(m_visibleCharacters[index]->name());
        }

        m_visibleCharacters.removeAt(index);
        m_characterNames.removeAt(index);
    }

    if (index < m_visibleCharacters.size()) setCurrentCharacter(index);
    else setCurrentCharacter(m_visibleCharacters.size() - 1);

    saveInactiveCharacters();

    emit charactersChanged();
}

void CharacterTool::loadInactiveCharacters()
{
    QSettings settings(sManager.getSetting(Setting::charactersPath) + "/settings.ini", QSettings::IniFormat);

    qDebug() << "CharacterTool:" << settings.fileName();

    m_inactiveCharacters = settings.value("inactive", {}).toStringList();
}

void CharacterTool::saveInactiveCharacters()
{
    QSettings settings(sManager.getSetting(Setting::charactersPath) + "/settings.ini", QSettings::IniFormat);

    settings.setValue("inactive", m_inactiveCharacters);
}

void CharacterTool::displayActiveCharacters(bool active)
{
    m_active = active;
    m_visibleCharacters.clear();
    m_characterNames.clear();

    for (auto c : m_characters)
    {
        if (c)
        {
            // XOR
            if (!active != !m_inactiveCharacters.contains(c->name()))
            {
                m_visibleCharacters.append(c);
                m_characterNames.append(c->name());
            }
        }
    }

    setCurrentCharacter(0);
    emit charactersChanged();
}
