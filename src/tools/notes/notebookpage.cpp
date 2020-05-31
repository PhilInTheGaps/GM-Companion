#include "notebookpage.h"

int NoteBookPage::editCount = 0;

NoteBookPage::NoteBookPage(const QString &name, int depth, QObject *chapter)
    : TreeItem(name, depth, false, chapter)
{
    setPriority(0);
}

QString NoteBookPage::content()
{
    if (!m_wasContentLoaded)
    {
        m_wasContentLoaded = true;
        emit loadPageContent();
    }

    return m_content;
}

void NoteBookPage::setContent(const QString &content, bool edited)
{
    m_content = content;

    if (edited)
    {
        setIsSaved(false);
        setWasEdited(true);
    }
    else
    {
        m_origContent = content;
    }

    m_editCount = ++editCount;

    emit generateHtml(content, m_editCount);
}

void NoteBookPage::setIsSaved(bool isSaved)
{
    m_isSaved = isSaved;

    if (isSaved) m_origContent = m_content;

    emit isSavedChanged();
}

void NoteBookPage::onContentLoaded(const QString &content)
{
    setContent(content, false);
    emit contentLoaded();
}

void NoteBookPage::onHtmlGenerated(const QString &html, int id)
{
    if (id != m_editCount) return;

    m_html = html;
    emit htmlGenerated();
}

void NoteBookPage::save()
{
    if (isSaved()) return;

    emit savePage();
}

void NoteBookPage::close()
{
    if (!isSaved())
    {
        m_content = m_origContent;
        setIsSaved(true);
    }

    setWasEdited(false);
    setIsCurrent(false);

    emit closePage();
}

void NoteBookPage::rename(const QString &newName)
{
    auto oldPath = path();

    auto correctName = newName;

    if (newName.lastIndexOf('.') < 0)
        correctName += ".md";

    setName(correctName);
    emit renamePage(oldPath);
}
