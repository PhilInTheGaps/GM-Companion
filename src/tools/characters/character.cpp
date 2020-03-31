#include "character.h"
#include "logging.h"
#include "filesystem/filemanager.h"

Character::Character(QString name, QObject *parent)
    : QObject(parent), m_name(name)
{
    qCDebug(gmCharactersCharacter()) << "Initializing new character:" << name << "...";
}

int Character::type() const
{
    if (m_files.size() < 1) return -1;

    if (m_files[0].name().endsWith("pdf")) return 1;

    if (m_files[0].name().endsWith("json")) return 2;

    return 0;
}

void Character::loadFiles()
{
    if (m_wasLoaded || m_folder.isEmpty() || !m_files.isEmpty())
    {
        emit fileListLoaded(m_files);
        return;
    }

    qCDebug(gmCharactersCharacter()) << "Loading files ...";

    m_wasLoaded = true;
    loadFileList();
}

void Character::loadFileList()
{
    auto requestId = FileManager::getUniqueRequestId();
    auto context   = new QObject;

    connect(FileManager::getInstance(), &FileManager::receivedFileList, context, [ = ](int id, QStringList fileNames) {
        if (requestId != id) return;

        for (auto fileName : fileNames)
        {
            m_files.append(CharacterFile(fileName, m_folder + "/" + fileName));
        }

        emit fileListLoaded(m_files);

        delete context;
    });

    FileManager::getInstance()->getFileList(requestId, m_folder, false);
}

void Character::loadFileData(int index)
{
    if ((index < 0) || (index > m_files.length() - 1)) return;

    if (!m_files[index].data().isEmpty())
    {
        qCDebug(gmCharactersImageViewer()) << "File data already loaded.";
        emit fileDataLoaded(index, m_files[index].data());
        return;
    }

    auto requestId = FileManager::getUniqueRequestId();
    auto context   = new QObject;

    connect(FileManager::getInstance(), &FileManager::receivedFile, context, [ = ](int id, QByteArray data) {
        if (requestId != id) return;

        m_files[index].setData(data);
        emit fileDataLoaded(index, data);

        delete context;
    });

    FileManager::getInstance()->getFile(requestId, m_files[index].path());
}
