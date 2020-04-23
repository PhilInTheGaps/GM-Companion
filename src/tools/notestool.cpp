#include "notestool.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "filesystem/filemanager.h"
#include <QQmlContext>

NotesTool::NotesTool(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent)
{
    qCDebug(gmNotesTool()) << "Loading Notes Tool ...";

    engine->rootContext()->setContextProperty("notes_tool", this);

    connect(FileManager::getInstance(), &FileManager::receivedFileList, this, &NotesTool::onReceivedFileList);

    updateChapters();
}

// Returns list of all available chapters
QStringList NotesTool::chapters()
{
    return m_chapters;
}

// Get all Chapters from Notes path
void NotesTool::updateChapters()
{
    qCDebug(gmNotesTool()) << "Updating Notes Chapters ...";

    m_folderListRequestId = FileManager::getUniqueRequestId();
    FileManager::getInstance()->getFileList(m_folderListRequestId, SettingsManager::getPath("notes"), true);
}

QStringList NotesTool::pages()
{
    return m_pages;
}

void NotesTool::updatePages()
{
    qCDebug(gmNotesTool()) << "Updating Notes Pages ...";

    m_pageListRequestId = FileManager::getUniqueRequestId();
    FileManager::getInstance()->getFileList(m_pageListRequestId, SettingsManager::getPath("notes") + "/" + m_currentChapter, false);
}

// Returns the current chapter
QString NotesTool::currentChapter()
{
    return m_currentChapter;
}

// Set current Chapter
void NotesTool::setCurrentChapter(QString chapter)
{
    m_currentChapter = chapter;
    emit currentChapterChanged();
}

// Returns the current page
QString NotesTool::currentPage()
{
    return m_currentPage;
}

// Set current page
void NotesTool::setCurrentPage(QString page)
{
    m_currentPage = page;
    m_currentContent.clear();
    emit currentPageChanged();

    loadPageContent();
}

void NotesTool::loadPageContent()
{
    auto path = SettingsManager::getPath("notes") + "/" + m_currentChapter + "/" + m_currentPage + ".txt";

    m_contentRequestId = FileManager::getUniqueRequestId();
    FileManager::getInstance()->getFile(m_contentRequestId, path);
}

// Save current page to file
void NotesTool::saveCurrentPageContent(QString content)
{
    m_currentContent = content;

    auto path = SettingsManager::getPath("notes") + "/" + m_currentChapter + "/" + m_currentPage + ".txt";
    auto data = content.toUtf8();

    FileManager::getInstance()->saveFile(path, data);
}

void NotesTool::addChapter(QString chapter)
{
    if (chapter.length() > 0)
    {
        QDir d(SettingsManager::getPath("notesPath") + "/" + chapter);

        if (!d.exists())
        {
            d.mkpath(d.absolutePath());

            updateChapters();
            emit chaptersChanged();

            m_currentChapter = chapter;
            emit currentChapterChanged();
        }
    }
}

void NotesTool::addPage(QString page)
{
    if (page.length() < 1) return;

    auto pages = m_pages;
    pages.append(page);
    pages.sort();
    loadPages(pages);
}

void NotesTool::deleteChapter(QString chapter)
{
    auto path = SettingsManager::getPath("notes") + "/" + chapter;

    FileManager::getInstance()->deleteFile(path);

    m_chapters.removeOne(chapter);
    emit chaptersChanged();

    if (m_chapters.size() > 0) m_currentChapter = m_chapters.at(0);
    else m_currentChapter = "";

    emit currentChapterChanged();
}

void NotesTool::deletePage(QString page)
{
    auto path = SettingsManager::getPath("notes") + "/" + m_currentChapter + "/" + page + ".txt";

    auto pages = m_pages;

    pages.removeOne(page);
    loadPages(pages);

    if (m_pages.size() > 0) m_currentPage = m_pages.at(0);
    else m_currentPage = "";
    emit currentPageChanged();
}

void NotesTool::loadChapters(QStringList folders)
{
    m_chapters.clear();

    for (QString chapter : folders)
    {
        m_chapters.append(chapter);
    }

    emit chaptersChanged();
}

void NotesTool::loadPages(QStringList files)
{
    m_pages.clear();

    for (QString page : files)
    {
        m_pages.append(page.replace(".txt", ""));
    }

    emit pagesChanged();
}

void NotesTool::onReceivedFileList(int requestId, QStringList files)
{
    if (requestId == m_folderListRequestId)
    {
        loadChapters(files);
        return;
    }

    if (requestId == m_pageListRequestId)
    {
        loadPages(files);
        return;
    }
}

void NotesTool::onReceivedFile(int requestId, QByteArray data)
{
    if (requestId == m_contentRequestId)
    {
        m_currentContent = data;
        emit currentPageChanged();
        return;
    }
}
