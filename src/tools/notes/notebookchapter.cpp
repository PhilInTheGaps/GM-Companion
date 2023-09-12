#include "notebookchapter.h"

NoteBookChapter::NoteBookChapter(const QString &name, QObject *parent) : NoteBookChapter(name, 1, parent)
{
}

NoteBookChapter::NoteBookChapter(const QString &name, int depth, QObject *parent) : TreeItem(name, depth, true, parent)
{
    connect(this, &TreeItem::isOpenChanged, this, &NoteBookChapter::onIsOpenChanged);
}

auto NoteBookChapter::pages() const -> QList<NoteBookPage *>
{
    QList<NoteBookPage *> pages;
    pages.reserve(childItems().size());

    foreach (auto *child, childItems())
    {
        auto *page = qobject_cast<NoteBookPage *>(child);
        if (page) pages.append(page);
    }

    return pages;
}

void NoteBookChapter::onPagesLoaded()
{
    m_werePagesLoaded = true;
    emit childItemsChanged();
}

void NoteBookChapter::create(const QString &type, const QString &name)
{
    const auto creatableTypes = creatables();

    if (type == creatableTypes.constFirst())
    {
        emit createPage(name);
    }
}

void NoteBookChapter::rename(const QString &newName)
{
    auto oldPath = path();
    name(newName);

    emit renameChapter(oldPath);
}

void NoteBookChapter::onIsOpenChanged()
{
    if (isOpen() && !m_werePagesLoaded)
    {
        emit loadPages();
    }
}
