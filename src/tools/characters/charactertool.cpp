#include "charactertool.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "filesystem/filemanager.h"

#include <QSettings>
#include <QTemporaryFile>
#include <QJsonArray>

CharacterTool::CharacterTool(QQmlApplicationEngine *engine, QObject *parent)
    : AbstractTool(parent)
{
    qDebug() << "Loading Character Tool ...";

    m_imageViewer = new CharacterImageViewer;
    m_dsa5Viewer  = new CharacterDSA5Viewer(engine);

    engine->rootContext()->setContextProperty("character_tool", this);
    engine->rootContext()->setContextProperty("character_image_viewer", m_imageViewer);
    engine->rootContext()->setContextProperty("character_dsa5_viewer", m_dsa5Viewer);

    connect(m_imageViewer, &CharacterImageViewer::categoryChanged, [ = ]() { if (m_currentViewer == m_imageViewer) emit categoryChanged(); });

    // connect(m_dsa5Viewer, &CharacterImageViewer::categoryChanged,    [ = ]()
    // { if (m_currentViewer == m_dsa5Viewer) emit categoryChanged(); });

    connect(m_imageViewer, &CharacterImageViewer::categoriesChanged, [ = ]() { if (m_currentViewer == m_imageViewer) emit categoriesChanged(); });

    // connect(m_dsa5Viewer, &CharacterDSA5Viewer::categoriesChanged, [ = ]()
    // { if (m_currentViewer == m_dsa5Viewer) emit categoriesChanged(); });

    connect(FileManager::getInstance(), &FileManager::receivedFile,     this, &CharacterTool::receivedFile);
    connect(FileManager::getInstance(), &FileManager::receivedFileList, this, &CharacterTool::receivedFileList);
}

auto CharacterTool::characters() const -> QStringList
{
    QStringList names;

    for (auto *character : m_characters)
    {
        if (!m_active != !m_inactiveCharacters.contains(character->name()))
        {
            names.append(character->name());
        }
    }

    return names;
}

void CharacterTool::updateCharacter()
{
    //    if (!m_currentCharacter || !m_currentViewer) return;

    //    if (m_currentCharacter->type() < 2)
    //    {
    //        m_imageViewer->updateImages();
    //        m_imageViewer->setCurrentCategory(0);
    //    }
}

void CharacterTool::setCurrentCharacter(int index)
{
    auto characterNames = characters();

    if ((index < 0) || (index > characterNames.length() - 1)) return;

    auto name = characterNames[index];
    m_currentCharacter = nullptr;

    for (int i = 0; i < m_characters.length(); i++)
    {
        auto *character = m_characters[i];

        if (character && (index >= i) && (character->name() == name))
        {
            m_currentCharacter = m_characters[i];
        }
    }

    if (m_currentCharacter)
    {
        if (m_currentCharacter->type() < 2) m_currentViewer = m_imageViewer;
        else m_currentViewer = m_dsa5Viewer;

        m_currentViewer->setCharacter(m_currentCharacter);
    }

    emit currentCharacterChanged();
    emit pageIndexChanged();
}

void CharacterTool::displayActiveCharacters(bool active)
{
    m_active = active;
    emit charactersChanged();
}

void CharacterTool::loadData()
{
    if (m_isDataLoaded) return;

    m_isDataLoaded = true;
    m_loadInactiveRequestId = FileManager::getUniqueRequestId();
    FileManager::getInstance()->getFile(m_loadInactiveRequestId, SettingsManager::getPath("characters") + "/inactive.json");
}

void CharacterTool::setCurrentCategory(int index)
{
    if (!m_currentViewer) return;

    m_currentViewer->setCurrentCategory(index);
}

void CharacterTool::toggleCharacterActive(int index)
{
    auto characterNames = characters();

    if ((index < 0) || (index > characterNames.length() - 1)) return;

    auto name = characterNames[index];

    if (m_active) m_inactiveCharacters.append(name);
    else m_inactiveCharacters.removeAll(name);

    if (index < characterNames.size() - 1) setCurrentCharacter(index);
    else setCurrentCharacter(characterNames.size() - 2);

    saveInactiveCharacters();
    emit charactersChanged();
}

void CharacterTool::receivedFile(int id, const QByteArray& data)
{
    // Load inactive characters
    if (id == m_loadInactiveRequestId)
    {
        loadInactiveCharacters(data);
        return;
    }

    // Convert old .ini file to json
    if (id == m_convertFileRequestId)
    {
        convertSettingsFile(data);
        return;
    }
}

/**
 * @brief Load list of inactive characters from json array
 */
void CharacterTool::loadInactiveCharacters(const QByteArray& data)
{
    if (data.isEmpty())
    {
        qCDebug(gmCharactersTool()) << "Inactive characters file data is empty, maybe old .ini file exists, trying to convert ...";
        m_convertFileRequestId = FileManager::getUniqueRequestId();
        FileManager::getInstance()->getFile(m_convertFileRequestId, SettingsManager::getPath("characters") + "/settings.ini");
        return;
    }

    qCDebug(gmCharactersTool()) << "Loading inactive characters ...";

    m_inactiveCharacters.clear();

    auto array = QJsonDocument::fromJson(data).array();

    for (auto entry : array) m_inactiveCharacters.append(entry.toString());

    loadCharacters();
}

/**
 * @brief Write all inactive characters to a json file
 */
void CharacterTool::saveInactiveCharacters()
{
    qCDebug(gmCharactersTool()) << "Saving inactive characters ...";

    auto filePath = SettingsManager::getPath("characters") + "/inactive.json";
    auto array    = QJsonArray::fromStringList(m_inactiveCharacters);

    FileManager::getInstance()->saveFile(filePath, QJsonDocument(array).toJson());
}

/**
 * @brief Read values from old settings.ini file and save as new inactive.json
 * file
 */
void CharacterTool::convertSettingsFile(const QByteArray& data)
{
    if (!data.isEmpty())
    {
        auto filePath = SettingsManager::getPath("characters") + "/settings.ini";
        qCDebug(gmCharactersTool()) << "Converting old .ini file at" << filePath;

        QTemporaryFile file;
        file.open();
        file.write(data);
        file.close();

        QSettings settings(file.fileName(), QSettings::IniFormat);
        m_inactiveCharacters = settings.value("inactive", {}).toStringList();
        saveInactiveCharacters();

        qCDebug(gmCharactersTool()) << "Deleting old .ini file.";
        FileManager::getInstance()->deleteFile(filePath);
    }

    loadCharacters();
}

void CharacterTool::receivedFileList(int id, const QStringList& files)
{
    if (id == m_loadCharacterFilesRequestId)
    {
        receivedCharacterFiles(files);
        return;
    }

    if (id == m_loadCharacterFoldersRequestId)
    {
        receivedCharacterFolders(files);
        return;
    }
}

void CharacterTool::loadCharacters()
{
    qCDebug(gmCharactersTool()) << "Loaded inactive character list, now loading character files and folders ...";

    m_loadCharacterFilesRequestId   = FileManager::getUniqueRequestId();
    m_loadCharacterFoldersRequestId = FileManager::getUniqueRequestId();

    FileManager::getInstance()->getFileList(m_loadCharacterFilesRequestId, SettingsManager::getPath("characters"), false);
    FileManager::getInstance()->getFileList(m_loadCharacterFoldersRequestId, SettingsManager::getPath("characters"), true);
}

void CharacterTool::receivedCharacterFolders(const QStringList& folders)
{
    qCDebug(gmCharactersTool()) << "Received character folders";

    auto basePath = SettingsManager::getPath("characters");

    for (const auto& folder : folders)
    {
        auto *character = new Character(folder);
        character->setFolder(basePath + "/" + folder);
        m_characters.append(character);
    }

    emit charactersChanged();
}

void CharacterTool::receivedCharacterFiles(const QStringList& files)
{
    qCDebug(gmCharactersTool()) << "Received character files";

    for (const auto& file : files)
    {
        if (file.endsWith(".pdf"))
        {
            auto *character = new Character(file.left(file.lastIndexOf('.')));
            character->addFile(CharacterFile(file, SettingsManager::getPath("characters") + "/" + file));
            m_characters.append(character);
        }
    }

    emit charactersChanged();
}
