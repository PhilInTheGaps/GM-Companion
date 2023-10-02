#include "notebook.h"

NoteBook::NoteBook(const QString &name, QObject *parent) : NoteBookChapter(name, 0, parent)
{
}

auto NoteBook::chapters() const -> QList<NoteBookChapter *>
{
    QList<NoteBookChapter *> chapters;

    foreach (auto *child, childItems())
    {
        auto *chapter = qobject_cast<NoteBookChapter *>(child);
        if (chapter) chapters.append(chapter);
    }

    return chapters;
}

void NoteBook::onChaptersLoaded()
{
    emit childItemsChanged();
}

void NoteBook::create(const QString &type, const QString &name)
{
    const auto creatableTypes = creatables();

    if (type == creatableTypes.at(0))
    {
        emit createChapter(name);
    }
    else if (type == creatableTypes.at(1))
    {
        emit createPage(name);
    }
}

void NoteBook::onIsOpenChanged()
{
    if (!isOpen()) return;

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
