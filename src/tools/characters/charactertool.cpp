#include "charactertool.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "filesystem/file.h"
#include "utils/utils.h"
#include "utils/fileutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QSettings>
#include <QTemporaryFile>
#include <QJsonArray>

using namespace AsyncFuture;

CharacterTool::CharacterTool(QQmlApplicationEngine *engine, QObject *parent)
    : AbstractTool(parent)
{
    qDebug() << "Loading Character Tool ...";

    m_imageViewer = new CharacterImageViewer(this);
    m_dsa5Viewer  = new CharacterDSA5Viewer(engine, this);

    engine->rootContext()->setContextProperty("character_tool", this);
    engine->rootContext()->setContextProperty("character_image_viewer", m_imageViewer);
    engine->rootContext()->setContextProperty("character_dsa5_viewer", m_dsa5Viewer);

    connect(m_imageViewer, &CharacterImageViewer::categoryChanged, [ = ]() { if (m_currentViewer == m_imageViewer) emit categoryChanged(); });

    // connect(m_dsa5Viewer, &CharacterImageViewer::categoryChanged,    [ = ]()
    // { if (m_currentViewer == m_dsa5Viewer) emit categoryChanged(); });

    connect(m_imageViewer, &CharacterImageViewer::categoriesChanged, [ = ]() { if (m_currentViewer == m_imageViewer) emit categoriesChanged(); });

    // connect(m_dsa5Viewer, &CharacterDSA5Viewer::categoriesChanged, [ = ]()
    // { if (m_currentViewer == m_dsa5Viewer) emit categoriesChanged(); });
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

    if (!Utils::isInBounds(characterNames, index)) return;

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

    const auto filePath = FileUtils::fileInDir("inactive.json", SettingsManager::getPath("characters"));
    observe(Files::File::getDataAsync(filePath)).subscribe([this](Files::FileDataResult *result) {
        loadInactiveCharacters(result->data());
        result->deleteLater();
    });
}

void CharacterTool::setCurrentCategory(int index)
{
    if (!m_currentViewer) return;

    m_currentViewer->setCurrentCategory(index);
}

void CharacterTool::toggleCharacterActive(int index)
{
    auto characterNames = characters();

    if (!Utils::isInBounds(characterNames, index)) return;

    auto name = characterNames[index];

    if (m_active) m_inactiveCharacters.append(name);
    else m_inactiveCharacters.removeAll(name);

    if (index < characterNames.size() - 1) setCurrentCharacter(index);
    else setCurrentCharacter(characterNames.size() - 2);

    saveInactiveCharacters();
    emit charactersChanged();
}

/**
 * @brief Load list of inactive characters from json array
 */
void CharacterTool::loadInactiveCharacters(const QByteArray& data)
{
    if (data.isEmpty())
    {
        qCDebug(gmCharactersTool()) << "Inactive characters file data is empty, maybe old .ini file exists, trying to convert ...";

        const auto filePath = FileUtils::fileInDir("settings.ini", SettingsManager::getPath("characters"));
        observe(Files::File::getDataAsync(filePath)).subscribe([this](Files::FileDataResult *result) {
            convertSettingsFile(result->data());
            result->deleteLater();
        });
        return;
    }

    qCDebug(gmCharactersTool()) << "Loading inactive characters ...";

    m_inactiveCharacters.clear();

    const auto array = QJsonDocument::fromJson(data).array();

    for (const auto &entry : array)
    {
        m_inactiveCharacters.append(entry.toString());
    }

    loadCharacters();
}

/**
 * @brief Write all inactive characters to a json file
 */
void CharacterTool::saveInactiveCharacters()
{
    qCDebug(gmCharactersTool()) << "Saving inactive characters ...";

    const auto filePath = FileUtils::fileInDir("inactive.json", SettingsManager::getPath("characters"));
    const auto data     = QJsonDocument(QJsonArray::fromStringList(m_inactiveCharacters)).toJson();

    Files::File::saveAsync(filePath, data);
}

/**
 * @brief Read values from old settings.ini file and save as new inactive.json
 * file
 */
void CharacterTool::convertSettingsFile(const QByteArray& data)
{
    if (!data.isEmpty())
    {
        const auto filePath = FileUtils::fileInDir("settings.ini", SettingsManager::getPath("characters"));
        qCDebug(gmCharactersTool()) << "Converting old .ini file at" << filePath;

        QTemporaryFile file;
        file.open();
        file.write(data);
        file.close();

        QSettings settings(file.fileName(), QSettings::IniFormat);
        m_inactiveCharacters = settings.value("inactive", {}).toStringList();
        saveInactiveCharacters();

        qCDebug(gmCharactersTool()) << "Deleting old .ini file.";
        Files::File::deleteAsync(filePath);
    }

    loadCharacters();
}

void CharacterTool::loadCharacters()
{
    qCDebug(gmCharactersTool()) << "Loaded inactive character list, now loading character files and folders ...";

    const auto dir = SettingsManager::getPath("characters");
    observe(Files::File::listAsync(dir, true, true)).subscribe([this](Files::FileListResult *result) {
        receivedCharacterFiles(result->files());
        receivedCharacterFolders(result->folders());
        result->deleteLater();
    });
}

void CharacterTool::receivedCharacterFolders(const QStringList& folders)
{
    qCDebug(gmCharactersTool()) << "Received character folders";

    const auto basePath = SettingsManager::getPath("characters");

    for (const auto& folder : folders)
    {
        auto *character = new Character(folder, this);
        character->folder(FileUtils::fileInDir(folder, basePath));
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
            const auto filePath = FileUtils::fileInDir(file, SettingsManager::getPath("characters"));
            auto *character = new Character(file.left(file.lastIndexOf('.')), this);
            character->addFile(new CharacterFile(file, filePath, this));
            m_characters.append(character);
        }
    }

    emit charactersChanged();
}
