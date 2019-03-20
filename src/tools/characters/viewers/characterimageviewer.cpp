#include "characterimageviewer.h"

#include <QDebug>
#include <QImage>
#include <QBuffer>

#ifdef Q_OS_LINUX
# include <poppler/qt5/poppler-qt5.h>
#endif // ifdef Q_OS_LINUX

CharacterImageViewer::CharacterImageViewer()
{
    qDebug() << "Loading CharacterImageViewer ...";
}

void CharacterImageViewer::setCharacter(Character *character)
{
    m_categories.clear();
    m_image.clear();
    m_categoryIndex = 0;

    m_currentCharacter = character;

    if (m_currentCharacter)
    {
        updateImages();
        setCurrentCategory(0);
        return;
    }

    emit categoryChanged();
    emit categoriesChanged();
}

void CharacterImageViewer::updateImages()
{
    m_categories.clear();

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
                m_categories.append(tr("Page") + " " + QString::number(i));
            }

            delete doc;
            #endif // ifdef Q_OS_LINUX
        }
        else
        {
            for (auto i : m_currentCharacter->files())
            {
                QString temp = i.name().right(i.name().length() - i.name().lastIndexOf('/') - 1);
                m_categories.append(temp.left(temp.lastIndexOf('.')));
            }
        }
    }

    emit categoriesChanged();
}

void CharacterImageViewer::setCurrentCategory(int index)
{
    m_image         = "";
    m_categoryIndex = 0;

    if (m_currentCharacter)
    {
        if (m_currentCharacter->type() == 1) // PDF
        {
            setPDFImage(index);
        }
        else if ((index > -1) && (index < m_currentCharacter->files().size()))
        {
            // For images
            QString p = m_currentCharacter->files()[index].path();
            m_image         = p.startsWith("http") ? p : "file:///" + p;
            m_categoryIndex = index;
        }
    }

    emit categoryChanged();
}

void CharacterImageViewer::setPDFImage(int index)
{
#ifdef Q_OS_LINUX
    Poppler::Document *doc = Poppler::Document::load(m_currentCharacter->files()[0].path());

    if (!doc || doc->isLocked())
    {
        emit categoryChanged();
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

            m_image         = QString("data:image/jpg;base64,") + QString::fromLatin1(bArray.toBase64().data());
            m_categoryIndex = index;
        }

        delete page;
    }

    delete doc;
#endif // ifdef Q_OS_LINUX
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
