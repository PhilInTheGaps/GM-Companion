#include "notessaveload.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "filesystem/filemanager.h"

#include <QBuffer>
#include <QPdfWriter>

NotesSaveLoad::NotesSaveLoad(QObject *parent) : QObject(parent)
{
    connect(FileManager::getInstance(), &FileManager::receivedFile, this, &NotesSaveLoad::onReceivedFile);
    connect(FileManager::getInstance(), &FileManager::receivedFileList, this, &NotesSaveLoad::onReceivedFileList);
}

/**
 * Load book directories in notes path
 */
void NotesSaveLoad::loadBooks()
{
    qCDebug(gmNotesSaveLoad()) << "Loading note books ...";

    auto directory = SettingsManager::getPath("notes");

    m_booksRequestId = FileManager::getUniqueRequestId();
    FileManager::getInstance()->getFileList(m_booksRequestId, directory, true);
}

/**
 * Load chapter directories in book directory
 */
void NotesSaveLoad::loadChapters()
{
    auto *book = qobject_cast<NoteBook*>(sender());
    if (!book) return;

    auto directory = SettingsManager::getPath("notes") + '/' + book->path();

    qCDebug(gmNotesSaveLoad()) << "Loading chapters in" << directory;

    auto requestId = FileManager::getUniqueRequestId();
    m_chapterRequests.insert(requestId, book);

    FileManager::getInstance()->getFileList(requestId, directory, true);
}

/**
 * Get page files in chapter directory
 */
void NotesSaveLoad::loadPages()
{
    auto *chapter = qobject_cast<NoteBookChapter*>(sender());
    if (!chapter) return;

    auto directory = SettingsManager::getPath("notes") + '/' + chapter->path();

    qCDebug(gmNotesSaveLoad()) << "Loading pages in" << directory;

    auto requestId = FileManager::getUniqueRequestId();
    m_pageRequests.insert(requestId, chapter);

    FileManager::getInstance()->getFileList(requestId, directory, false);
}

/**
 * Load content from page file
 */
void NotesSaveLoad::loadPageContent()
{
    auto *page = qobject_cast<NoteBookPage*>(sender());
    if (!page) return;

    auto fileName = SettingsManager::getPath("notes") + '/' + page->path();

    qCDebug(gmNotesSaveLoad()) << "Loading page content of" << fileName;

    auto requestId = FileManager::getUniqueRequestId();
    m_contentRequests.insert(requestId, page);

    FileManager::getInstance()->getFile(requestId, fileName);
}

/**
 * Write page content to file
 */
void NotesSaveLoad::savePage()
{
    auto *page = qobject_cast<NoteBookPage*>(sender());
    if (!page) return;

    auto fileName = SettingsManager::getPath("notes") + '/' + page->path();

    qCDebug(gmNotesSaveLoad()) << "Saving page content of" << fileName;

    FileManager::getInstance()->saveFile(fileName, page->content().toUtf8());
    page->setIsSaved(true);
}

/**
 * Rename a chapter folder
 */
void NotesSaveLoad::renameChapter(const QString &oldPath)
{
    auto *chapter = qobject_cast<NoteBookChapter*>(sender());
    if (!chapter || oldPath == chapter->path()) return;

    const auto basePath = SettingsManager::getPath("notes");
    const auto old = basePath + "/" + oldPath;
    const auto newPath = basePath + "/" + chapter->path();

    qCDebug(gmNotesSaveLoad()) << "Renaming chapter or book" << chapter->path() << "to" << oldPath;

    FileManager::getInstance()->renameFolder(newPath, old);
}

/**
 * Rename a page file to it's new name
 */
void NotesSaveLoad::renamePage(const QString &oldPath)
{
    auto *page = qobject_cast<NoteBookPage*>(sender());
    if (!page || oldPath == page->path()) return;

    const auto basePath = SettingsManager::getPath("notes");
    const auto old = basePath + "/" + oldPath;
    const auto newPath = basePath + "/" + page->path();

    qCDebug(gmNotesSaveLoad()) << "Renaming page" << page->path() << "to" << oldPath;

    FileManager::getInstance()->renameFile(newPath, old);
}

/**
 * Delete chapter folder
 */
void NotesSaveLoad::deleteChapter()
{
    auto *chapter = qobject_cast<NoteBookChapter*>(sender());
    if (!chapter) return;

    qCDebug(gmNotesSaveLoad()) << "Deleting chapter or book" << chapter->path();

    const auto path = SettingsManager::getPath("notes") + "/" + chapter->path();

    for (auto *page : chapter->pages()) page->close();

    chapter->deleteLater();

    FileManager::getInstance()->deleteFile(path);
}

/**
 * Delete page file
 */
void NotesSaveLoad::deletePage()
{
    auto *page = qobject_cast<NoteBookPage*>(sender());
    if (!page) return;

    qCDebug(gmNotesSaveLoad()) << "Deleting page" << page->path();

    const auto path = SettingsManager::getPath("notes") + "/" + page->path();

    page->close();
    page->deleteLater();

    FileManager::getInstance()->deleteFile(path);
}

/**
 * Create a new book folder
 */
void NotesSaveLoad::createBook(const QString &name, QObject *root)
{
    if (name.isEmpty()) return;

    qCDebug(gmNotesSaveLoad()) << "Creating book" << name;

    auto path = SettingsManager::getPath("notes") + "/" + name;
    FileManager::getInstance()->createFolder(path);

    buildBooks({ name }, qobject_cast<TreeItem*>(root));
}

/**
 * Create a new chapter folder
 */
void NotesSaveLoad::createChapter(const QString &name)
{
    if (name.isEmpty()) return;

    auto *book = qobject_cast<NoteBook*>(sender());
    if (!book) return;

    qCDebug(gmNotesSaveLoad()) << "Creating chapter" << name << "in book" << book->name();

    // Check if chapter with name already exists
    for (auto *child : qAsConst(book->children()))
    {
        const auto *chapter = qobject_cast<NoteBookChapter*>(child);
        if (chapter && chapter->name() == name) return;
    }

    auto localPath = "/" + book->path() + "/" + name;
    auto path = SettingsManager::getPath("notes") + localPath;

    FileManager::getInstance()->createFolder(path);
    buildChapters({ name }, book);
}

/**
 * Create a new page file
 */
void NotesSaveLoad::createPage(const QString &name)
{
    if (name.isEmpty() || name == ".") return;

    auto *chapter = qobject_cast<NoteBookChapter*>(sender());
    if (!chapter) return;

    qCDebug(gmNotesSaveLoad()) << "Creating page" << name;

    // Check if page with name already exists
    for (auto *child : qAsConst(chapter->children()))
    {
        const auto *page = qobject_cast<NoteBookPage*>(child);
        if (page && page->name() == name) return;
    }

    auto correctName = name;

    if (name.endsWith(".")) correctName = correctName.left(correctName.length() - 1);

    if (!correctName.endsWith(".md") && !correctName.endsWith(".txt") && !correctName.endsWith(".markdown"))
        correctName += ".md";

    auto localPath = "/" + chapter->path() + "/" + correctName;
    auto path = SettingsManager::getPath("notes") + localPath;

    FileManager::getInstance()->saveFile(path, "# " + name.toUtf8() + "\n");

    if (!buildPage(correctName, chapter))
    {
        qCWarning(gmNotesSaveLoad()) << "Error: Could not build page!";
    }
}

/**
 * Save the page as a PDF
 */
void NotesSaveLoad::exportPage(NoteBookPage *page, QTextDocument *document)
{
    if (!document) return;

    qCDebug(gmNotesSaveLoad()) << "Exporting page as pdf ...";

    auto *buffer = new QBuffer;
    auto *writer = new QPdfWriter(buffer);

    writer->setTitle(page->name());
    writer->setCreator("GM-Companion");

    auto pageSize = QPageSize(QPageSize::A4);
    auto pageOrientation = QPageLayout::Portrait;
    auto pageMargins = QMarginsF();
    auto pageUnits = QPageLayout::Millimeter;

    QPageLayout pageLayout(pageSize, pageOrientation, pageMargins, pageUnits);
    writer->setPageLayout(pageLayout);

    if (buffer->open(QIODevice::WriteOnly))
    {
        document->print(writer);
        buffer->close();

        auto filePath = NotesSaveLoad::getPdfPath(page);

        FileManager::getInstance()->saveFile(filePath, buffer->data());
    }
    else
    {
        qCWarning(gmNotesSaveLoad()) << "Error: Could not opoen buffer for pdf export.";
    }


    buffer->deleteLater();
    writer->deleteLater();
}

/**
 * Construct book objects from folder list
 */
void NotesSaveLoad::buildBooks(const QStringList &folders, TreeItem *root)
{
    qCDebug(gmNotesSaveLoad()) << "Building books:" << folders;

    bool replace = false;

    if (!root)
    {
        replace = true;
        root = new TreeItem("/", 0, true, this);
        root->setIsOpen(true);
    }

    for (const auto &folder : folders)
    {
        auto *book = new NoteBook(folder, root);

        connect(book, &NoteBook::loadChapters, this, &NotesSaveLoad::loadChapters);
        connect(book, &NoteBook::loadPages, this, &NotesSaveLoad::loadPages);
        connect(book, &NoteBook::createPage, this, &NotesSaveLoad::createPage);
        connect(book, &NoteBook::createChapter, this, &NotesSaveLoad::createChapter);
        connect(book, &NoteBook::renameChapter, this, &NotesSaveLoad::renameChapter);
        connect(book, &NoteBook::deleteChapter, this, &NotesSaveLoad::deleteChapter);
    }

    if (replace) emit booksLoaded(root);
    else root->childItemAdded();
}

/**
 * Construct chapter objects from folder list
 */
void NotesSaveLoad::buildChapters(const QStringList &folders, NoteBook *book)
{
    if (!book) return;

    qCDebug(gmNotesSaveLoad()) << "Building chapters in book" << book->name() << folders;

    for (const auto &folder : folders)
    {
        bool exists = false;

        for (const auto *chapter : book->chapters())
        {
            if (chapter->name() == folder)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            auto *chapter = new NoteBookChapter(folder, book);
            connect(chapter, &NoteBookChapter::loadPages, this, &NotesSaveLoad::loadPages);
            connect(chapter, &NoteBookChapter::createPage, this, &NotesSaveLoad::createPage);
            connect(chapter, &NoteBookChapter::renameChapter, this, &NotesSaveLoad::renameChapter);
            connect(chapter, &NoteBookChapter::deleteChapter, this, &NotesSaveLoad::deleteChapter);
        }
    }

    book->onChaptersLoaded();
}

/**
 * Construct page objects from file list
 */
void NotesSaveLoad::buildPages(const QStringList &files, NoteBookChapter *chapter)
{
    if (!chapter) return;

    qCDebug(gmNotesSaveLoad()) << "Building pages in chapter" << chapter->name() << files;

    QList<NoteBookPage*> pages;

    for (const auto &file : files)
    {
        bool exists = false;

        for (const auto *page : chapter->pages())
        {
            if (page->name() == file)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            auto page = buildPage(file, chapter, false);
            if (page) pages.append(page);
        }
    }

    chapter->onPagesLoaded();
    emit pagesLoaded(pages);
}

/**
 * Construct a single page
 */
NoteBookPage* NotesSaveLoad::buildPage(const QString &name, NoteBookChapter *chapter, bool emitSignal)
{
    if (name.endsWith(".md") || name.endsWith(".txt") || name.endsWith(".markdown"))
    {
        auto *page = new NoteBookPage(name, chapter->depth() + 1, chapter);

        connect(page, &NoteBookPage::loadPageContent, this, &NotesSaveLoad::loadPageContent);
        connect(page, &NoteBookPage::savePage, this, &NotesSaveLoad::savePage);
        connect(page, &NoteBookPage::renamePage, this, &NotesSaveLoad::renamePage);
        connect(page, &NoteBookPage::deletePage, this, &NotesSaveLoad::deletePage);

        if (emitSignal)
        {
            chapter->onPagesLoaded();
            emit pagesLoaded({ page });
        }

        return page;
    }

    return nullptr;
}

/**
 * Get the path were the PDF version of the page should
 * be saved at.
 * (Basically rename the file to *.pdf inside the notes path)
 */
QString NotesSaveLoad::getPdfPath(NoteBookPage *page)
{
    auto fileName = page->path();

    QStringList endings = { ".md", ".txt", ".markdown" };

    for (const auto &ending : endings)
    {
        if (fileName.endsWith(ending))
        {
            fileName = fileName.left(fileName.length() - ending.length());
            break;
        }
    }

    fileName += ".pdf";

    return SettingsManager::getPath("notes") + "/" + fileName;
}

/**
 * When a list of files is received from the FileManager.
 * Check if the request belongs to the SaveLoad and call
 * the relevant function.
 */
void NotesSaveLoad::onReceivedFileList(int requestId, const QStringList &files)
{
    if (requestId == m_booksRequestId)
    {
        buildBooks(files, nullptr);
        return;
    }

    if (m_chapterRequests.contains(requestId))
    {
        buildChapters(files, m_chapterRequests.take(requestId));
        return;
    }

    if (m_pageRequests.contains(requestId))
    {
        buildPages(files, m_pageRequests.take(requestId));
        return;
    }
}

/**
 * Called when data from the FileManager is received.
 * Check if the data belongs to a page request and if yes, load it.
 */
void NotesSaveLoad::onReceivedFile(int requestId, const QByteArray &data)
{
    if (m_contentRequests.contains(requestId))
    {
        auto *page = m_contentRequests.take(requestId);
        page->onContentLoaded(data);
    }
}
