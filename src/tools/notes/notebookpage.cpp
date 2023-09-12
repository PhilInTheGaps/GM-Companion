#include "notebookpage.h"

using namespace Qt::Literals::StringLiterals;

int NoteBookPage::editCount = 0;

NoteBookPage::NoteBookPage(const QString &name, int depth, QObject *parent) : TreeItem(name, depth, false, parent)
{
    priority(0);
}

auto NoteBookPage::content() -> QString
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
        keepOpen(true);
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

    keepOpen(false);
    isCurrent(false);

    emit closePage();
}

void NoteBookPage::rename(const QString &newName)
{
    auto oldPath = path();
    auto correctName = newName;

    if (newName.lastIndexOf('.') < 0) correctName += ".md"_L1;

    name(correctName);
    emit renamePage(oldPath);
}
