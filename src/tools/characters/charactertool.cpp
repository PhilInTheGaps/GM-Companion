#include "charactertool.h"
#include "src/functions.h"

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QImage>
#include <QBuffer>

#ifdef Q_OS_LINUX
# include <poppler/qt5/poppler-qt5.h>
#endif // ifdef Q_OS_LINUX

CharacterTool::CharacterTool(FileManager *fManager, QObject *parent)
    : QObject(parent), fileManager(fManager)
{
    qDebug() << "Loading Character Tool ...";

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
    m_images.clear();

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

void CharacterTool::receivedFiles(int reqId, QList<CharacterFile>files)
{
    if (m_currentCharacter && (m_currentCharacter->requestId() == reqId))
    {
        m_currentCharacter->setFiles(files);
        updateImages();
        setImage(0);
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
        updateImages();
        setImage(0);
        emit currentCharacterChanged();

        if ((m_currentCharacter->files().size() == 0) && (m_currentCharacter->requestId() == -1))
        {
            fileManager->getCharacterFileManager()->findFiles(m_currentCharacter);
        }

        return;
    }

    setImage(0);
    m_currentCharacter = nullptr;
    m_images.clear();
    emit currentCharacterChanged();
}

void CharacterTool::updateImages()
{
    m_images.clear();

    if (m_currentCharacter && (m_currentCharacter->files().size() > 0))
    {
        // PDFs
        if (m_currentCharacter->type() == 1)
        {
            #ifdef Q_OS_LINUX
            Poppler::Document *doc = Poppler::Document::load(m_currentCharacter->files()[0].path());

            if (!doc || doc->isLocked())
            {
                delete doc;
                return;
            }

            for (int i = 0; i < doc->numPages(); i++)
            {
                m_images.append(tr("Page") + " " + QString::number(i));
            }

            delete doc;
            #endif // ifdef Q_OS_LINUX
            return;
        }

        for (auto i : m_currentCharacter->files())
        {
            QString temp = i.name().right(i.name().length() - i.name().lastIndexOf('/') - 1);
            m_images.append(temp.left(temp.lastIndexOf('.')));
        }
    }
}

void CharacterTool::setImage(int index)
{
    m_image      = "";
    m_imageIndex = 0;

    if (m_currentCharacter)
    {
        if (m_currentCharacter->type() == 1) // PDF
        {
            #ifdef Q_OS_LINUX
            Poppler::Document *doc = Poppler::Document::load(m_currentCharacter->files()[0].path());

            if (!doc || doc->isLocked())
            {
                emit imageChanged();
                delete doc;
                return;
            }

            if (index < doc->numPages())
            {
                Poppler::Page *page = doc->page(index);

                if (page)
                {
                    QImage image = page->renderToImage(200, 200);
                    QByteArray bArray;
                    QBuffer    buffer(&bArray);
                    buffer.open(QIODevice::WriteOnly);
                    image.save(&buffer, "JPEG");
                    buffer.close();

                    m_image      = QString("data:image/jpg;base64,") + QString::fromLatin1(bArray.toBase64().data());
                    m_imageIndex = index;
                }

                delete page;
            }

            delete doc;
            #endif // ifdef Q_OS_LINUX
        }
        else if ((index > -1) && (index < m_currentCharacter->files().size()))
        {
            // For images
            QString p = m_currentCharacter->files()[index].path();
            m_image      = p.startsWith("http") ? p : "file:///" + p;
            m_imageIndex = index;
        }
    }

    emit imageChanged();
}

void CharacterTool::nextImage(bool right)
{
    if (right)
    {
        if (m_imageIndex < m_images.size() - 1) setImage(m_imageIndex + 1);
        else setImage(0);
    }
    else
    {
        if (m_imageIndex > 0) setImage(m_imageIndex - 1);
        else setImage(m_images.size() - 1);
    }
}

void CharacterTool::toggleCharacterActive(int index)
{
    if ((index > -1) && (index < m_visibleCharacters.size()))
    {
        if (m_active)
        {
            m_inactiveCharacters.append(m_visibleCharacters[index]->name());
            m_visibleCharacters.removeAt(index);
            m_characterNames.removeAt(index);
        }
        else
        {
            m_inactiveCharacters.removeOne(m_visibleCharacters[index]->name());
            m_visibleCharacters.removeAt(index);
            m_characterNames.removeAt(index);
        }
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
