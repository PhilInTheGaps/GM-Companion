#include "notessaveload.h"
#include "filesystem/file.h"
#include "filesystem/results/filedataresult.h"
#include "filesystem/results/filelistresult.h"
#include "filesystem/results/fileresult.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include <QBuffer>
#include <QLoggingCategory>
#include <QPdfWriter>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmNotesSaveLoad, "gm.notes.saveload")

/**
 * Load book directories in notes path
 */
void NotesSaveLoad::loadBooks()
{
    qCDebug(gmNotesSaveLoad()) << "Loading note books ...";

    const auto directory = SettingsManager::getPath(u"notes"_s);

    Files::File::listAsync(directory, false, true).then(this, [this](std::shared_ptr<Files::FileListResult> result) {
        buildBooks(result->folders(), nullptr);
    });
}

/**
 * Load chapter directories in book directory
 */
void NotesSaveLoad::loadChapters()
{
    auto *book = qobject_cast<NoteBook *>(sender());
    if (!book) return;

    const auto directory = FileUtils::fileInDir(book->path(), SettingsManager::getPath(u"notes"_s));

    qCDebug(gmNotesSaveLoad()) << "Loading chapters in" << directory;

    Files::File::listAsync(directory, false, true)
        .then(this,
              [this, book](std::shared_ptr<Files::FileListResult> result) { buildChapters(result->folders(), *book); });
}

/**
 * Get page files in chapter directory
 */
void NotesSaveLoad::loadPages()
{
    auto *chapter = qobject_cast<NoteBookChapter *>(sender());
    if (!chapter) return;

    const auto directory = FileUtils::fileInDir(chapter->path(), SettingsManager::getPath(u"notes"_s));

    qCDebug(gmNotesSaveLoad()) << "Loading pages in" << directory;

    Files::File::listAsync(directory, true, false)
        .then(this, [this, chapter](std::shared_ptr<Files::FileListResult> result) {
            buildPages(result->files(), *chapter);
        });
}

/**
 * Load content from page file
 */
void NotesSaveLoad::loadPageContent()
{
    auto *page = qobject_cast<NoteBookPage *>(sender());
    if (!page) return;

    const auto fileName = FileUtils::fileInDir(page->path(), SettingsManager::getPath(u"notes"_s));

    qCDebug(gmNotesSaveLoad()) << "Loading page content of" << fileName;

    Files::File::getDataAsync(fileName).then(
        this, [page](std::shared_ptr<Files::FileDataResult> result) { page->onContentLoaded(result->data()); });
}

/**
 * Write page content to file
 */
void NotesSaveLoad::savePage()
{
    auto *page = qobject_cast<NoteBookPage *>(sender());
    if (!page) return;

    const auto fileName = FileUtils::fileInDir(page->path(), SettingsManager::getPath(u"notes"_s));

    qCDebug(gmNotesSaveLoad()) << "Saving page content of" << fileName;

    Files::File::saveAsync(fileName, page->content().toUtf8())
        .then(this, [page](std::shared_ptr<Files::FileResult> result) { page->setIsSaved(result->success()); });
}

/**
 * Rename a chapter folder
 */
void NotesSaveLoad::renameChapter(const QString &oldPath)
{
    const auto *chapter = qobject_cast<NoteBookChapter *>(sender());
    if (!chapter || oldPath == chapter->path()) return;

    const auto basePath = SettingsManager::getPath(u"notes"_s);
    const auto old = FileUtils::fileInDir(oldPath, basePath);
    const auto newPath = FileUtils::fileInDir(chapter->path(), basePath);

    qCDebug(gmNotesSaveLoad()) << "Renaming chapter or book" << chapter->path() << "to" << oldPath;

    Files::File::moveAsync(old, newPath);
}

/**
 * Rename a page file to it's new name
 */
void NotesSaveLoad::renamePage(const QString &oldPath)
{
    const auto *page = qobject_cast<NoteBookPage *>(sender());
    if (!page || oldPath == page->path()) return;

    const auto basePath = SettingsManager::getPath(u"notes"_s);
    const auto old = FileUtils::fileInDir(oldPath, basePath);
    const auto newPath = FileUtils::fileInDir(page->path(), basePath);

    qCDebug(gmNotesSaveLoad()) << "Renaming page" << page->path() << "to" << oldPath;

    Files::File::moveAsync(old, newPath);
}

/**
 * Delete chapter folder
 */
void NotesSaveLoad::deleteChapter()
{
    auto *chapter = qobject_cast<NoteBookChapter *>(sender());
    if (!chapter) return;

    qCDebug(gmNotesSaveLoad()) << "Deleting chapter or book" << chapter->path();

    const auto path = FileUtils::fileInDir(chapter->path(), SettingsManager::getPath(u"notes"_s));

    const auto pages = chapter->pages();
    for (auto *page : pages)
        page->close();

    delete chapter;

    Files::File::deleteAsync(path);
}

/**
 * Delete page file
 */
void NotesSaveLoad::deletePage()
{
    auto *page = qobject_cast<NoteBookPage *>(sender());
    if (!page) return;

    qCDebug(gmNotesSaveLoad()) << "Deleting page" << page->path();

    const auto path = FileUtils::fileInDir(page->path(), SettingsManager::getPath(u"notes"_s));

    page->close();
    delete page;

    Files::File::deleteAsync(path);
}

/**
 * Create a new book folder
 */
void NotesSaveLoad::createBook(const QString &name, TreeItem *root)
{
    if (name.isEmpty()) return;

    qCDebug(gmNotesSaveLoad()) << "Creating book" << name;

    const auto path = FileUtils::fileInDir(name, SettingsManager::getPath(u"notes"_s));
    Files::File::createDirAsync(path).then(
        this, [this, name, root](std::shared_ptr<Files::FileResult>) { buildBooks({name}, root); });
}

/**
 * Create a new chapter folder
 */
void NotesSaveLoad::createChapter(const QString &name)
{
    if (name.isEmpty()) return;

    auto *book = qobject_cast<NoteBook *>(sender());
    if (!book) return;

    qCDebug(gmNotesSaveLoad()) << "Creating chapter" << name << "in book" << book->name();

    // Check if chapter with name already exists
    foreach (auto *child, book->children())
    {
        const auto *chapter = qobject_cast<NoteBookChapter *>(child);
        if (chapter && chapter->name() == name) return;
    }

    const auto localPath = FileUtils::fileInDir(name, book->path());
    const auto path = FileUtils::fileInDir(localPath, SettingsManager::getPath(u"notes"_s));
    Files::File::createDirAsync(path).then(
        this, [this, name, book](std::shared_ptr<Files::FileResult>) { buildChapters({name}, *book); });
}

/**
 * Create a new page file
 */
void NotesSaveLoad::createPage(const QString &name)
{
    if (name.isEmpty() || name == "."_L1 || name.contains(".."_L1)) return;

    auto *chapter = qobject_cast<NoteBookChapter *>(sender());
    if (!chapter) return;

    qCDebug(gmNotesSaveLoad()) << "Creating page" << name;

    // Check if page with name already exists
    foreach (auto *child, chapter->children())
    {
        const auto *page = qobject_cast<NoteBookPage *>(child);
        if (page && page->name() == name) return;
    }

    auto correctName = name;

    if (name.endsWith("."_L1)) correctName = correctName.left(correctName.length() - 1);

    if (!correctName.endsWith(".md"_L1) && !correctName.endsWith(".txt"_L1) && !correctName.endsWith(".markdown"_L1))
        correctName += ".md"_L1;

    const auto localPath = FileUtils::fileInDir(correctName, chapter->path());
    const auto path = FileUtils::fileInDir(localPath, SettingsManager::getPath(u"notes"_s));
    const auto data = "# " + name.toUtf8() + "\n";

    Files::File::saveAsync(path, data)
        .then(this, [this, correctName, chapter](std::shared_ptr<Files::FileResult> result) {
            if (!result)
            {
                qCCritical(gmNotesSaveLoad()) << "Error: Could not save page, something went very wrong!";
                return;
            }

            if (!result->success() || !buildPage(correctName, *chapter))
            {
                qCWarning(gmNotesSaveLoad()) << "Error: Could not build page!";
            }
        });
}

/**
 * Save the page as a PDF
 */
void NotesSaveLoad::exportPage(NoteBookPage &page, const QTextDocument &document)
{
    qCDebug(gmNotesSaveLoad()) << "Exporting page as pdf ...";

    auto buffer = std::make_unique<QBuffer>();
    auto writer = std::make_unique<QPdfWriter>(buffer.get());

    writer->setTitle(page.name());
    writer->setCreator(u"GM-Companion"_s);

    auto pageSize = QPageSize(QPageSize::A4);
    auto pageOrientation = QPageLayout::Portrait;
    auto pageMargins = QMarginsF();
    auto pageUnits = QPageLayout::Millimeter;

    QPageLayout pageLayout(pageSize, pageOrientation, pageMargins, pageUnits);
    writer->setPageLayout(pageLayout);

    if (buffer->open(QIODevice::WriteOnly))
    {
        document.print(writer.get());
        buffer->close();

        const auto filePath = NotesSaveLoad::getPdfPath(page);
        Files::File::saveAsync(filePath, buffer->data());
    }
    else
    {
        qCWarning(gmNotesSaveLoad()) << "Error: Could not open buffer for pdf export.";
    }
}

/**
 * Construct book objects from folder list
 */
void NotesSaveLoad::buildBooks(const QStringList &folders, TreeItem *root)
{
    qCDebug(gmNotesSaveLoad()) << "Building books:" << folders;

    bool replaceRoot = false;

    if (!root)
    {
        replaceRoot = true;
        root = new TreeItem(u"/"_s, 0, true, this);
        root->isOpen(true);
    }

    foreach (const auto &folder, folders)
    {
        const auto *book = new NoteBook(folder, root);

        connect(book, &NoteBook::loadChapters, this, &NotesSaveLoad::loadChapters);
        connect(book, &NoteBook::loadPages, this, &NotesSaveLoad::loadPages);
        connect(book, &NoteBook::createPage, this, &NotesSaveLoad::createPage);
        connect(book, &NoteBook::createChapter, this, &NotesSaveLoad::createChapter);
        connect(book, &NoteBook::renameChapter, this, &NotesSaveLoad::renameChapter);
        connect(book, &NoteBook::deleteChapter, this, &NotesSaveLoad::deleteChapter);
    }

    if (!folders.isEmpty())
    {
        root->onChildItemAdded();
    }

    if (replaceRoot) emit booksLoaded(root);
}

/**
 * Construct chapter objects from folder list
 */
void NotesSaveLoad::buildChapters(const QStringList &folders, NoteBook &book) const
{
    qCDebug(gmNotesSaveLoad()) << "Building chapters in book" << book.name() << folders;

    foreach (const auto &folder, folders)
    {
        bool exists = false;

        foreach (const auto *chapter, book.chapters())
        {
            if (chapter->name() == folder)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            const auto *chapter = new NoteBookChapter(folder, &book);
            connect(chapter, &NoteBookChapter::loadPages, this, &NotesSaveLoad::loadPages);
            connect(chapter, &NoteBookChapter::createPage, this, &NotesSaveLoad::createPage);
            connect(chapter, &NoteBookChapter::renameChapter, this, &NotesSaveLoad::renameChapter);
            connect(chapter, &NoteBookChapter::deleteChapter, this, &NotesSaveLoad::deleteChapter);
        }
    }

    book.onChaptersLoaded();
}

/**
 * Construct page objects from file list
 */
void NotesSaveLoad::buildPages(const QStringList &files, NoteBookChapter &chapter)
{
    qCDebug(gmNotesSaveLoad()) << "Building pages in chapter" << chapter.name() << files;

    QList<NoteBookPage *> pages;

    for (const auto &file : files)
    {
        bool exists = false;

        foreach (const auto *page, chapter.pages())
        {
            if (page->name() == file)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            auto *page = buildPage(file, chapter, false);
            if (page) pages.append(page);
        }
    }

    chapter.onPagesLoaded();
    emit pagesLoaded(pages);
}

/**
 * Construct a single page
 */
auto NotesSaveLoad::buildPage(const QString &name, NoteBookChapter &chapter, bool emitSignal) -> NoteBookPage *
{
    if (name.endsWith(".md"_L1) || name.endsWith(".txt"_L1) || name.endsWith(".markdown"_L1))
    {
        auto *page = new NoteBookPage(name, chapter.depth() + 1, &chapter);

        connect(page, &NoteBookPage::loadPageContent, this, &NotesSaveLoad::loadPageContent);
        connect(page, &NoteBookPage::savePage, this, &NotesSaveLoad::savePage);
        connect(page, &NoteBookPage::renamePage, this, &NotesSaveLoad::renamePage);
        connect(page, &NoteBookPage::deletePage, this, &NotesSaveLoad::deletePage);

        if (emitSignal)
        {
            chapter.onPagesLoaded();
            emit pagesLoaded({page});
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
auto NotesSaveLoad::getPdfPath(const NoteBookPage &page) -> QString
{
    auto fileName = page.path();

    const QStringList endings = {".md", ".txt", ".markdown"};

    for (const auto &ending : endings)
    {
        if (fileName.endsWith(ending))
        {
            fileName = fileName.left(fileName.length() - ending.length());
            break;
        }
    }

    fileName += ".pdf"_L1;

    return FileUtils::fileInDir(fileName, SettingsManager::getPath(u"notes"_s));
}
