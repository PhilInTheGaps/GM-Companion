#include "characterimageviewer.h"

#include "logging.h"
#include <QPixmap>
#include <QBuffer>
#include "utils/utils.h"

CharacterImageViewer::CharacterImageViewer()
{
}

CharacterImageViewer::~CharacterImageViewer()
{
    if (m_pdfDocument) delete m_pdfDocument;
}

void CharacterImageViewer::setCharacter(Character *character)
{
    // House cleaning
    m_categories.clear();
    m_image.clear();
    m_categoryIndex = 0;

    disconnect(m_currentCharacter, &Character::fileListLoaded, this, &CharacterImageViewer::onCharacterFileListLoaded);
    disconnect(m_currentCharacter, &Character::fileDataLoaded, this, &CharacterImageViewer::onCharacterFileDataLoaded);

    // Load new character
    m_currentCharacter = character;

    emit categoryChanged();
    emit categoriesChanged();

    if (m_currentCharacter)
    {
        connect(m_currentCharacter, &Character::fileListLoaded, this, &CharacterImageViewer::onCharacterFileListLoaded);
        connect(m_currentCharacter, &Character::fileDataLoaded, this, &CharacterImageViewer::onCharacterFileDataLoaded);
        m_currentCharacter->loadFiles();
    }
}

void CharacterImageViewer::onCharacterFileListLoaded(QList<CharacterFile>files)
{
    qCDebug(gmCharactersImageViewer()) << "Character files list was loaded.";

    for (auto file : files) m_categories.append(file.name());
    emit categoriesChanged();

    if (!m_categories.isEmpty()) setCurrentCategory(0);
}

void CharacterImageViewer::onCharacterFileDataLoaded(int index, QByteArray data)
{
    qCDebug(gmCharactersImageViewer()) << "File data was loaded.";

    if (m_categoryIndex != index) return;


    switch (m_currentCharacter->type())
    {
    case 0: // Folder with images
        loadImage(data);
        break;

    case 1: // PDF
        loadPDF(index, data);
        break;

    default:
        qCCritical(gmCharactersImageViewer()) << "Character type" << m_currentCharacter->type() << "of character" << m_currentCharacter->name() << "not implemented.";
    }
}

void CharacterImageViewer::loadImage(QByteArray data)
{
    qCDebug(gmCharactersImageViewer()) << "Loading image from data";

    QPixmap pixmap;

    if (!pixmap.loadFromData(data))
    {
        qCWarning(gmCharactersImageViewer()) << "Could not load image.";
        return;
    }

    m_image = Utils::stringFromImage(pixmap);
    emit categoryChanged();
}

void CharacterImageViewer::loadPDF(int index, QByteArray data)
{
    qCDebug(gmCharactersImageViewer()) << "Loading pdf document";

    if (m_pdfDocument && (m_currentCharacter->name() == m_pdfCharacter))
    {
        qCDebug(gmCharactersImageViewer()) << "Document already loaded.";
    }
    else
    {
        if (m_pdfDocument) delete m_pdfDocument;

        m_pdfCharacter = m_currentCharacter->name();
        m_pdfDocument  = Poppler::Document::loadFromData(data);

        if (!m_pdfDocument || m_pdfDocument->isLocked())
        {
            qCWarning(gmCharactersImageViewer()) << "Could not load pdf document";
            return;
        }
    }

    loadPDFCategories();
    setPDFPage(index);
}

void CharacterImageViewer::loadPDFCategories()
{
    m_categories.clear();

    for (int i = 0; i < m_pdfDocument->numPages(); i++)
    {
        m_categories.append(tr("Page") + " " + QString::number(i + 1));
    }

    emit categoriesChanged();
}

void CharacterImageViewer::setPDFPage(int index)
{
    qCDebug(gmCharactersImageViewer()) << "Loading pdf page at index" << index;

    if (!m_pdfDocument) return;

    if (index < m_pdfDocument->numPages())
    {
        Poppler::Page *page = m_pdfDocument->page(index);

        if (page)
        {
            m_image = Utils::stringFromImage(QPixmap::fromImage(page->renderToImage(200, 200)));
        }

        delete page;
    }

    emit categoryChanged();
}

void CharacterImageViewer::setCurrentCategory(int index)
{
    qCDebug(gmCharactersImageViewer()) << "Changing page:" << index;

    m_image = "";

    m_categoryIndex = index;
    emit categoryChanged();

    qCDebug(gmCharactersImageViewer()) << "Loading page data ...";

    if ((m_currentCharacter->type() == 1) && (index > 0))
    {
        setPDFPage(index);
    }
    else
    {
        m_currentCharacter->loadFileData(index);
    }
}

void CharacterImageViewer::nextImage(bool right)
{
    if (right)
    {
        if (m_categoryIndex < m_categories.size() - 1) setCurrentCategory(m_categoryIndex + 1);
        else setCurrentCategory(0);
    }
    else
    {
        if (m_categoryIndex > 0) setCurrentCategory(m_categoryIndex - 1);
        else setCurrentCategory(m_categories.size() - 1);
    }
}
