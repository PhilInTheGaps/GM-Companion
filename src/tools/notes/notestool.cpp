#include "notestool.h"
#include "utils/stringutils.h"
#include <QDesktopServices>
#include <QFile>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmNotesTool, "gm.notes.tool")

NotesTool::NotesTool(QObject *parent) : AbstractTool(parent), m_saveLoad(this), m_markdownHighlighter(this)
{
    connect(this, &NotesTool::loadBooks, &m_saveLoad, &NotesSaveLoad::loadBooks);
    connect(&m_saveLoad, &NotesSaveLoad::booksLoaded, this, &NotesTool::onNoteBooksLoaded);
    connect(&m_saveLoad, &NotesSaveLoad::pagesLoaded, this, &NotesTool::onPagesLoaded);
}

auto NotesTool::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> NotesTool *
{
    Q_UNUSED(jsEngine)
    return new NotesTool(qmlEngine);
}

void NotesTool::setQmlTextDoc(QQuickTextDocument *qmlTextDoc)
{
    m_qmlTextDoc = qmlTextDoc;
    emit qmlTextDocChanged();

    if (!m_qmlTextDoc || !m_qmlTextDoc->textDocument()) return;

    // Load style sheet
    QFile f(u":/notes/style.css"_s);
    if (f.open(QIODevice::ReadOnly))
    {
        const QString style = f.readAll();
        m_qmlTextDoc->textDocument()->setDefaultStyleSheet(style);
        f.close();
    }

    connect(m_qmlTextDoc->textDocument(), &QTextDocument::contentsChanged, this, &NotesTool::onDocumentEdited);

    m_markdownHighlighter.setDocument(m_qmlTextDoc->textDocument());

    if (m_currentPage) displayPageContent();
}

void NotesTool::setCurrentPage(NoteBookPage *page)
{
    if (m_currentPage)
    {
        disconnect(m_currentPage, &NoteBookPage::contentLoaded, this, &NotesTool::onPageContentLoaded);
        disconnect(m_currentPage, &NoteBookPage::htmlGenerated, this, &NotesTool::onPageHtmlLoaded);
        m_currentPage->isCurrent(false);
    }

    if (page)
    {
        m_currentPage = page;
        m_currentPage->isCurrent(true);
        connect(m_currentPage, &NoteBookPage::contentLoaded, this, &NotesTool::onPageContentLoaded);
        connect(m_currentPage, &NoteBookPage::htmlGenerated, this, &NotesTool::onPageHtmlLoaded);

        displayPageContent();
    }

    emit currentPageChanged();
}

void NotesTool::setEditMode(bool editMode)
{
    m_editMode = editMode;

    if (m_currentPage)
    {
        displayPageContent();
    }

    emit editModeChanged();
}

void NotesTool::loadData()
{
    if (isDataLoaded()) return;

    setIsDataLoaded(true);
    emit loadBooks();
}

/**
 * En-/Decrypt page using ROT13
 */
void NotesTool::encrypt()
{
    if (!m_currentPage) return;

    m_currentPage->setContent(StringUtils::rot13(m_currentPage->content()));
    onPageContentLoaded();
}

/**
 * Export the current page as PDF document
 */
void NotesTool::exportPdf()
{
    if (editMode() || !m_currentPage || !m_qmlTextDoc) return;

    auto *textDoc = m_qmlTextDoc->textDocument();
    if (!textDoc) return;

    m_saveLoad.exportPage(*m_currentPage, *textDoc);
}

/**
 * Open a link in browser.
 *
 * @note Maybe this can later be used for integration
 * with other tools.
 */
void NotesTool::linkClicked(const QString &link) const
{
    QDesktopServices::openUrl(link);
}

void NotesTool::createBook(const QString &name)
{
    if (!doesBookExist(name)) m_saveLoad.createBook(name, notesModel());
}

/**
 * Connect signals of newly loaded pages
 */
void NotesTool::onPagesLoaded(const QList<NoteBookPage *> &pages)
{
    foreach (const auto *page, pages)
    {
        connect(page, &NoteBookPage::selected, this, &NotesTool::onPageClicked);
        connect(page, &NoteBookPage::closePage, this, &NotesTool::onClosePage);
    }
}

/**
 * Close all pages that were not edited.
 * No openedPagesChanged() signal is emitted,
 * but should be done manually after use.
 */
void NotesTool::closeUnneededPages()
{
    foreach (auto *page, a_openedPages)
    {
        if (!page->keepOpen())
        {
            a_openedPages.removeOne(page);
        }
    }
}

/**
 * If in edit mode, show plain text, else html
 */
void NotesTool::displayPageContent()
{
    if (editMode()) onPageContentLoaded();
    else
        onPageHtmlLoaded();
}

/**
 * Find out if a book already exists
 */
auto NotesTool::doesBookExist(const QString &name) const -> bool
{
    const auto books = a_notesModel->children();

    return std::any_of(books.constBegin(), books.constEnd(), [name](const QObject *object) {
        const auto *book = qobject_cast<const NoteBook *>(object);
        return book && book->name() == name;
    });
}

void NotesTool::onNoteBooksLoaded(TreeItem *root)
{
    notesModel(root);
}

void NotesTool::onPageClicked()
{
    auto *page = qobject_cast<NoteBookPage *>(sender());

    if (!a_openedPages.contains(page))
    {
        closeUnneededPages();
        a_openedPages.append(page);
        emit openedPagesChanged();
    }

    setCurrentPage(page);
}

void NotesTool::onClosePage()
{
    qCDebug(gmNotesTool()) << "Closing page ...";

    int i = 0;

    foreach (auto *page, a_openedPages)
    {
        if (page->objectName() == sender()->objectName())
        {
            a_openedPages.removeOne(page);

            if (a_openedPages.isEmpty())
            {
                m_currentPage = nullptr;
                if (m_qmlTextDoc) m_qmlTextDoc->textDocument()->clear();
                break;
            }

            if (i >= a_openedPages.length()) i = a_openedPages.length() - 1;

            auto *newPage = a_openedPages.at(i);

            if (newPage) newPage->toggle();

            break;
        }

        i++;
    }

    emit openedPagesChanged();
}

void NotesTool::onPageContentLoaded()
{
    if (editMode() && m_qmlTextDoc && m_qmlTextDoc->textDocument())
    {
        m_cursorPosition = m_currentPage->cursorPosition();

        auto *doc = m_qmlTextDoc->textDocument();
        doc->setDefaultFont(QFont(u"Mono"_s));
        doc->setPlainText(m_currentPage->content());
    }
}

void NotesTool::onPageHtmlLoaded()
{
    if (!editMode() && m_qmlTextDoc && m_qmlTextDoc->textDocument())
    {
        m_cursorPosition = m_currentPage->cursorPosition();

        if (!m_currentPage->content().isEmpty())
        {
            auto *doc = m_qmlTextDoc->textDocument();
            doc->setDefaultFont(QFont(u"sans serif"_s));
            doc->setHtml(m_currentPage->html());
        }
    }
}

void NotesTool::onDocumentEdited()
{
    if (!m_currentPage || !m_qmlTextDoc) return;

    auto *doc = m_qmlTextDoc->textDocument();

    if (editMode())
    {
        auto plainText = doc->toPlainText();

        if (plainText != m_currentPage->content()) m_currentPage->setContent(plainText);
    }

    // Have we just replaced the document content?
    if (!doc->isUndoAvailable() && !doc->isRedoAvailable())
    {
        emit setCursorPosition(m_cursorPosition);
    }
}
