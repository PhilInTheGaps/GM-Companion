#include "charactertool.h"
#include "filesystem/file.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/fileutils.h"
#include "utils/utils.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QQmlContext>
#include <QSettings>
#include <QTemporaryFile>

using namespace Qt::Literals::StringLiterals;
using namespace AsyncFuture;

Q_LOGGING_CATEGORY(gmCharactersTool, "gm.characters.tool")

CharacterTool::CharacterTool(QQmlApplicationEngine *engine, QObject *parent) : AbstractTool(parent)
{
    qCDebug(gmCharactersTool()) << "Loading Character Tool ...";

    m_imageViewer = new CharacterImageViewer(this);

    engine->rootContext()->setContextProperty(u"character_tool"_s, this);
    engine->rootContext()->setContextProperty(u"character_image_viewer"_s, m_imageViewer);

    connect(m_imageViewer, &CharacterImageViewer::categoryChanged, this, [this]() {
        if (m_currentViewer == m_imageViewer) emit categoryChanged();
    });

    connect(m_imageViewer, &CharacterImageViewer::categoriesChanged, this, [this]() {
        if (m_currentViewer == m_imageViewer) emit categoriesChanged();
    });
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

void CharacterTool::updateCharacter() const
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
        auto *character = m_characters.at(i);

        if (character && (index >= i) && (character->name() == name))
        {
            m_currentCharacter = m_characters.at(i);
        }
    }

    if (m_currentCharacter)
    {
        if (m_currentCharacter->type() < 2) m_currentViewer = m_imageViewer;

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
    if (isDataLoaded()) return;

    setIsDataLoaded(true);

    const auto filePath = FileUtils::fileInDir(u"inactive.json"_s, SettingsManager::getPath(u"characters"_s));
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
    else
        m_inactiveCharacters.removeAll(name);

    if (index < characterNames.size() - 1) setCurrentCharacter(index);
    else
        setCurrentCharacter(characterNames.size() - 2);

    saveInactiveCharacters();
    emit charactersChanged();
}

/**
 * @brief Load list of inactive characters from json array
 */
void CharacterTool::loadInactiveCharacters(const QByteArray &data)
{
    if (data.isEmpty())
    {
        qCDebug(gmCharactersTool())
            << "Inactive characters file data is empty, maybe old .ini file exists, trying to convert ...";

        const auto filePath = FileUtils::fileInDir(u"settings.ini"_s, SettingsManager::getPath(u"characters"_s));
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
void CharacterTool::saveInactiveCharacters() const
{
    qCDebug(gmCharactersTool()) << "Saving inactive characters ...";

    const auto filePath = FileUtils::fileInDir(u"inactive.json"_s, SettingsManager::getPath(u"characters"_s));
    const auto data = QJsonDocument(QJsonArray::fromStringList(m_inactiveCharacters)).toJson();

    Files::File::saveAsync(filePath, data);
}

/**
 * @brief Read values from old settings.ini file and save as new inactive.json
 * file
 */
void CharacterTool::convertSettingsFile(const QByteArray &data)
{
    if (!data.isEmpty())
    {
        const auto filePath = FileUtils::fileInDir(u"settings.ini"_s, SettingsManager::getPath(u"characters"_s));
        qCDebug(gmCharactersTool()) << "Converting old .ini file at" << filePath;

        QTemporaryFile file;
        file.open();
        file.write(data);
        file.close();

        QSettings settings(file.fileName(), QSettings::IniFormat);
        m_inactiveCharacters = settings.value("inactive"_L1, {}).toStringList();
        saveInactiveCharacters();

        qCDebug(gmCharactersTool()) << "Deleting old .ini file.";
        Files::File::deleteAsync(filePath);
    }

    loadCharacters();
}

void CharacterTool::loadCharacters()
{
    qCDebug(gmCharactersTool()) << "Loaded inactive character list, now loading character files and folders ...";

    const auto dir = SettingsManager::getPath(u"characters"_s);
    observe(Files::File::listAsync(dir, true, true)).subscribe([this](Files::FileListResult *result) {
        receivedCharacterFiles(result->files());
        receivedCharacterFolders(result->folders());
        result->deleteLater();
    });
}

void CharacterTool::receivedCharacterFolders(const QStringList &folders)
{
    qCDebug(gmCharactersTool()) << "Received character folders";

    const auto basePath = SettingsManager::getPath(u"characters"_s);

    for (const auto &folder : folders)
    {
        auto *character = new Character(folder, this);
        character->folder(FileUtils::fileInDir(folder, basePath));
        m_characters.append(character);
    }

    emit charactersChanged();
}

void CharacterTool::receivedCharacterFiles(const QStringList &files)
{
    qCDebug(gmCharactersTool()) << "Received character files";

    for (const auto &file : files)
    {
        if (file.endsWith(".pdf"_L1))
        {
            const auto filePath = FileUtils::fileInDir(file, SettingsManager::getPath(u"characters"_s));
            auto *character = new Character(file.left(file.lastIndexOf('.')), this);
            character->addFile(new CharacterFile(file, filePath, this));
            m_characters.append(character);
        }
    }

    emit charactersChanged();
}
