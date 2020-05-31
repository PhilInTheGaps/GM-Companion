#include "notebook.h"

NoteBook::NoteBook(const QString& name, QObject *parent)
    : NoteBookChapter(name, 0, parent)
{}

auto NoteBook::chapters() -> QList<NoteBookChapter *>
{
    if (!m_wereChaptersLoaded)
    {
        m_wereChaptersLoaded = true;
        emit loadChapters();
    }

    return m_chapters;
}

void NoteBook::onChaptersLoaded()
{
    emit childItemsChanged();
}

void NoteBook::create(const QString &type, const QString &name)
{   
    const auto creatableTypes = creatables();

    if (type == creatableTypes[0])
    {
        emit createChapter(name);
    }
    else if (type == creatableTypes[1])
    {
        emit createPage(name);
    }
}

void NoteBook::onIsOpenChanged()
{
    if (isOpen())
    {
        if (!m_wereChaptersLoaded)
        {
            m_wereChaptersLoaded = true;
            emit loadChapters();
        }

        if (!m_werePagesLoaded)
        {
            m_werePagesLoaded = true;
            emit loadPages();
        }
    }
}
