#include "notestool.h"
#include "functions.h"
#include "settings/settingsmanager.h"
#include <QDebug>

NotesTool::NotesTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Notes Tool ...";

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
    qDebug() << "Updating Notes Chapters ...";

    m_chapters.clear();

    for (QString chapter : getFolders(SettingsManager::getPath("notesPath")))
    {
        if (!chapter.contains("."))
        {
            m_chapters.append(chapter);
        }
    }

    emit chaptersChanged();
}

QStringList NotesTool::pages()
{
    return m_pages;
}

void NotesTool::updatePages()
{
    qDebug() << "Updating Notes Pages ...";

    m_pages.clear();

    for (QString page : getFiles(SettingsManager::getPath("notesPath") + "/" + m_currentChapter))
    {
        m_pages.append(page.replace(".txt", ""));
    }

    emit pagesChanged();
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
    emit currentPageChanged();
}

// Save current page to file
void NotesTool::saveCurrentPageContent(QString content)
{
    QFile f(SettingsManager::getPath("notesPath") + "/" + m_currentChapter + "/" + m_currentPage + ".txt");

    if (f.exists())
    {
        f.open(QFile::WriteOnly);

        QTextStream stream(&f);
        stream.setCodec("UTF-8");

        QString output = content;
        stream << output;

        f.close();
    }
}

// Return the content of the current page
QString NotesTool::currentPageContent()
{
    QString content = "";

    QFile f(SettingsManager::getPath("notesPath") + "/" + m_currentChapter + "/" + m_currentPage + ".txt");

    if (f.exists())
    {
        f.open(QIODevice::ReadOnly);

        content = f.readAll();

        f.close();
    }

    return content;
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
    if (page.length() > 0)
    {
        QFile f(SettingsManager::getPath("notesPath") + "/" + m_currentChapter + "/" + page + ".txt");

        if (!f.exists())
        {
            f.open(QIODevice::WriteOnly);
            f.close();

            updatePages();
            emit pagesChanged();

            //            m_currentPage = page;
            emit currentPageChanged();
        }
    }
}

void NotesTool::deleteChapter(QString chapter)
{
    QDir d(SettingsManager::getPath("notesPath") + "/" + chapter);

    if (d.exists())
    {
        if (!d.rmdir(d.absolutePath()))
        {
            for (QString f : getFiles(d.absolutePath()))
            {
                QFile file(d.absolutePath() + "/" + f);
                file.remove();
            }

            d.rmdir(d.absolutePath());
        }

        updateChapters();
        emit chaptersChanged();

        if (m_chapters.size() > 0) m_currentChapter = m_chapters.at(0);
        else m_currentChapter = "";

        emit currentChapterChanged();
    }
}

void NotesTool::deletePage(QString page)
{
    QFile f(SettingsManager::getPath("notesPath") + "/" + m_currentChapter + "/" + page + ".txt");

    if (f.exists())
    {
        f.remove();

        updatePages();
        emit pagesChanged();

        if (m_pages.size() > 0) m_currentPage = m_pages.at(0);
        else m_currentPage = "";
        emit currentPageChanged();
    }
}

// Returns content ROT 13 encrypted
QString NotesTool::encrypt(QString content)
{
    return rot13(content);
}
