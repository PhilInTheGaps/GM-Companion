#include "characterfilemanager.h"
#include "src/functions.h"
#include <QDebug>

CharacterFileManager::CharacterFileManager(GoogleDrive *google, QObject *parent)
    : QObject(parent), googleDrive(google)
{
    qDebug() << "Loading CharacterFileManager ...";

    // Received list of character folders
    connect(googleDrive, &GoogleDrive::receivedSubfolderIDs, [ = ](int reqId, QList<GoogleFile>files) {
        if (!m_requestIDs.contains(reqId)) return;

        for (auto f : files)
        {
            auto character = new Character(f.fileName);
            character->setCloudId(f.id);
            m_characters.append(character);
        }

        emit receivedCharacters(m_characters);
        m_requestIDs.removeOne(reqId);
    });

    // Received list of images in character folder
    connect(google, qOverload<int, QList<GoogleFile> >(&GoogleDrive::receivedFileUrls), [ = ](int reqId, QList<GoogleFile>files) {
        qDebug() << "CharacterFileManager: Received some files!" << m_requestIDs << reqId;

        if (m_requestIDs.contains(reqId))
        {
            if ((files.size() > 0) && (files[0].parent == sManager.getSetting(Setting::googlePath, "characters")))
            { // PDFs
                for (auto f : files)
                {
                    if (f.fileName.endsWith("pdf"))
                    {
                        auto character = new Character(f.fileName.left(f.fileName.lastIndexOf('.')));
                        character->addFile(CharacterFile(f.fileName, f.weblink));
                        character->setCloudId(f.id);
                        m_characters.append(character);
                    }
                }

                emit receivedCharacters(m_characters);
            }
            else
            { // Images
                QList<CharacterFile>list;

                for (auto f : files) list.append(CharacterFile(f.fileName, f.weblink));

                emit receivedFiles(reqId, list);
            }
        }
    });
}

void CharacterFileManager::findCharacters(int mode)
{
    qDebug() << "CharacterFileManager: Finding characters ...";
    m_mode = mode;
    m_characters.clear();

    switch (m_mode)
    {
    case 0: // Local
        findLocalCharacters();
        break;

    case 1: // Google Drive
        m_requestIDs.append(googleDrive->getSubfolderIDs(sManager.getSetting(Setting::googlePath, "characters")));

        // PDFs currently disabled, will change in future
        // m_requestIDs.append(googleDrive->getFileUrls(sManager.getSetting(Setting::googlePath,
        // "characters")));
        break;

    default: return;
    }
}

void CharacterFileManager::findLocalCharacters()
{
    qDebug() << "CharacterFileManager: Finding local characters ...";

    QString path = sManager.getSetting(Setting::charactersPath);

    // Find image based characters
    for (auto folder : getFolders(path))
    {
        if (!folder.contains('.'))
        {
            auto c = new Character(folder);

            for (auto file : getFiles(path + "/" + folder))
            {
                if (file.endsWith("jpeg") || file.endsWith("jpg") || file.endsWith("png"))
                {
                    c->addFile(CharacterFile(file, path + "/" + folder + "/" + file));
                }
            }

            m_characters.append(c);
        }
    }

    // Find pdf characters
    for (auto file : getFiles(path))
    {
        if (file.endsWith("pdf") || file.endsWith("json"))
        {
            auto c = new Character(file.left(file.lastIndexOf('.')));
            c->addFile(CharacterFile(file, path + "/" + file));
            m_characters.append(c);
        }
    }

    emit receivedCharacters(m_characters);
}

void CharacterFileManager::findFiles(Character *character)
{
    if (m_mode == 1)
    {
        if (character->requestId() == -1)
        {
            int reqId = googleDrive->getFileUrls(character->cloudId());
            m_requestIDs.append(reqId);
            character->setRequestId(reqId);
        }
    }
}
