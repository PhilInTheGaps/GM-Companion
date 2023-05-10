#include "notessaveload.h"
#include "filesystem/file.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/fileutils.h"

#include <QBuffer>
#include <QPdfWriter>

using namespace AsyncFuture;

NotesSaveLoad::NotesSaveLoad(QObject *parent) : QObject(parent)
{
}

/**
 * Load book directories in notes path
 */
void NotesSaveLoad::loadBooks()
{
    qCDebug(gmNotesSaveLoad()) << "Loading note books ...";

    const auto directory = SettingsManager::getPath(QStringLiteral("notes"));

    observe(Files::File::listAsync(directory, false, true)).subscribe([this](Files::FileListResult *result) {
        buildBooks(result->folders(), nullptr);
        result->deleteLater();
    });
}

/**
 * Load chapter directories in book directory
 */
void NotesSaveLoad::loadChapters()
{
    auto *book = qobject_cast<NoteBook *>(sender());
    if (!book) return;

    const auto directory = FileUtils::fileInDir(book->path(), SettingsManager::getPath(QStringLiteral("notes")));

    qCDebug(gmNotesSaveLoad()) << "Loading chapters in" << directory;

    observe(Files::File::listAsync(directory, false, true)).subscribe([this, book](Files::FileListResult *result) {
        buildChapters(result->folders(), book);
        result->deleteLater();
    });
}

/**
 * Get page files in chapter directory
 */
void NotesSaveLoad::loadPages()
{
    auto *chapter = qobject_cast<NoteBookChapter *>(sender());
    if (!chapter) return;

    const auto directory = FileUtils::fileInDir(chapter->path(), SettingsManager::getPath(QStringLiteral("notes")));

    qCDebug(gmNotesSaveLoad()) << "Loading pages in" << directory;

    observe(Files::File::listAsync(directory, true, false)).subscribe([this, chapter](Files::FileListResult *result) {
        buildPages(result->files(), chapter);
        result->deleteLater();
    });
}

/**
 * Load content from page file
 */
void NotesSaveLoad::loadPageContent()
{
    auto *page = qobject_cast<NoteBookPage *>(sender());
    if (!page) return;

    const auto fileName = FileUtils::fileInDir(page->path(), SettingsManager::getPath(QStringLiteral("notes")));

    qCDebug(gmNotesSaveLoad()) << "Loading page content of" << fileName;

    observe(Files::File::getDataAsync(fileName)).subscribe([page](Files::FileDataResult *result) {
        page->onContentLoaded(result->data());
        result->deleteLater();
    });
}

/**
 * Write page content to file
 */
void NotesSaveLoad::savePage()
{
    auto *page = qobject_cast<NoteBookPage *>(sender());
    if (!page) return;

    const auto fileName = FileUtils::fileInDir(page->path(), SettingsManager::getPath(QStringLiteral("notes")));

    qCDebug(gmNotesSaveLoad()) << "Saving page content of" << fileName;

    Files::File::saveAsync(fileName, page->content().toUtf8());
    page->setIsSaved(true);
}

/**
 * Rename a chapter folder
 */
void NotesSaveLoad::renameChapter(const QString &oldPath)
{
    auto *chapter = qobject_cast<NoteBookChapter *>(sender());
    if (!chapter || oldPath == chapter->path()) return;

    const auto basePath = SettingsManager::getPath(QStringLiteral("notes"));
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
    auto *page = qobject_cast<NoteBookPage *>(sender());
    if (!page || oldPath == page->path()) return;

    const auto basePath = SettingsManager::getPath(QStringLiteral("notes"));
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

    const auto path = FileUtils::fileInDir(chapter->path(), SettingsManager::getPath(QStringLiteral("notes")));

    const auto pages = chapter->pages();
    for (auto *page : pages)
        page->close();

    chapter->deleteLater();

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

    const auto path = FileUtils::fileInDir(page->path(), SettingsManager::getPath(QStringLiteral("notes")));

    page->close();
    page->deleteLater();

    Files::File::deleteAsync(path);
}

/**
 * Create a new book folder
 */
void NotesSaveLoad::createBook(const QString &name, QObject *root)
{
    if (name.isEmpty()) return;

    qCDebug(gmNotesSaveLoad()) << "Creating book" << name;

    const auto path = FileUtils::fileInDir(name, SettingsManager::getPath(QStringLiteral("notes")));
    observe(Files::File::createDirAsync(path)).subscribe([this, name, root]() {
        buildBooks({name}, qobject_cast<TreeItem *>(root));
    });
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
    for (auto *child : qAsConst(book->children()))
    {
        const auto *chapter = qobject_cast<NoteBookChapter *>(child);
        if (chapter && chapter->name() == name) return;
    }

    const auto localPath = FileUtils::fileInDir(name, book->path());
    const auto path = FileUtils::fileInDir(localPath, SettingsManager::getPath(QStringLiteral("notes")));
    observe(Files::File::createDirAsync(path)).subscribe([this, name, book]() { buildChapters({name}, book); });
}

/**
 * Create a new page file
 */
void NotesSaveLoad::createPage(const QString &name)
{
    if (name.isEmpty() || name == QLatin1String(".")) return;

    auto *chapter = qobject_cast<NoteBookChapter *>(sender());
    if (!chapter) return;

    qCDebug(gmNotesSaveLoad()) << "Creating page" << name;

    // Check if page with name already exists
    for (auto *child : qAsConst(chapter->children()))
    {
        const auto *page = qobject_cast<NoteBookPage *>(child);
        if (page && page->name() == name) return;
    }

    auto correctName = name;

    if (name.endsWith(QLatin1String("."))) correctName = correctName.left(correctName.length() - 1);

    if (!correctName.endsWith(QLatin1String(".md")) && !correctName.endsWith(QLatin1String(".txt")) &&
        !correctName.endsWith(QLatin1String(".markdown")))
        correctName += QLatin1String(".md");

    const auto localPath = FileUtils::fileInDir(correctName, chapter->path());
    const auto path = FileUtils::fileInDir(localPath, SettingsManager::getPath(QStringLiteral("notes")));
    const auto data = "# " + name.toUtf8() + "\n";

    Files::File::saveAsync(path, data);

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

    auto *buffer = new QBuffer(this);
    auto *writer = new QPdfWriter(buffer);

    writer->setTitle(page->name());
    writer->setCreator(QStringLiteral("GM-Companion"));

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

        const auto filePath = NotesSaveLoad::getPdfPath(page);
        Files::File::saveAsync(filePath, buffer->data());
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
        root = new TreeItem(QStringLiteral("/"), 0, true, this);
        root->isOpen(true);
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
    else
        root->onChildItemAdded();
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
        const auto chapters = book->chapters();

        for (const auto *chapter : chapters)
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

    QList<NoteBookPage *> pages;

    for (const auto &file : files)
    {
        bool exists = false;
        const auto _pages = chapter->pages();

        for (const auto *page : _pages)
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
auto NotesSaveLoad::buildPage(const QString &name, NoteBookChapter *chapter, bool emitSignal) -> NoteBookPage *
{
    if (name.endsWith(QLatin1String(".md")) || name.endsWith(QLatin1String(".txt")) ||
        name.endsWith(QLatin1String(".markdown")))
    {
        auto *page = new NoteBookPage(name, chapter->depth() + 1, chapter);

        connect(page, &NoteBookPage::loadPageContent, this, &NotesSaveLoad::loadPageContent);
        connect(page, &NoteBookPage::savePage, this, &NotesSaveLoad::savePage);
        connect(page, &NoteBookPage::renamePage, this, &NotesSaveLoad::renamePage);
        connect(page, &NoteBookPage::deletePage, this, &NotesSaveLoad::deletePage);

        if (emitSignal)
        {
            chapter->onPagesLoaded();
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
auto NotesSaveLoad::getPdfPath(NoteBookPage *page) -> QString
{
    auto fileName = page->path();

    QStringList endings = {".md", ".txt", ".markdown"};

    for (const auto &ending : endings)
    {
        if (fileName.endsWith(ending))
        {
            fileName = fileName.left(fileName.length() - ending.length());
            break;
        }
    }

    fileName += QStringLiteral(".pdf");

    return FileUtils::fileInDir(fileName, SettingsManager::getPath(QStringLiteral("notes")));
}
