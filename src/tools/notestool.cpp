#include "notestool.h"
#include "src/functions.h"

#include <QDebug>

NotesTool::NotesTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Notes Tool ...";

    sManager = new SettingsManager;

    updateChapters();
}

// Returns list of all available chapters
QStringList NotesTool::chapters()
{
    return l_chapters;
}

// Get all Chapters from Notes path
void NotesTool::updateChapters()
{
    qDebug() << "Updating Notes Chapters ...";

    l_chapters.clear();

    for (QString chapter : getFolders(sManager->getSetting(Setting::notesPath)))
    {
        if (!chapter.contains("."))
        {
            l_chapters.append(chapter);
        }
    }

    emit chaptersChanged();
}

QStringList NotesTool::pages()
{
    return l_pages;
}

void NotesTool::updatePages()
{
    qDebug() << "Updating Notes Pages ...";

    l_pages.clear();

    for (QString page : getFiles(sManager->getSetting(Setting::notesPath) + "/" + l_currentChapter))
    {
        l_pages.append(page.replace(".txt", ""));
    }

    emit pagesChanged();
}

// Returns the current chapter
QString NotesTool::currentChapter()
{
    return l_currentChapter;
}

// Set current Chapter
void NotesTool::setCurrentChapter(QString chapter)
{
    l_currentChapter = chapter;
    emit currentChapterChanged();
}

// Returns the current page
QString NotesTool::currentPage()
{
    return l_currentPage;
}

// Set current page
void NotesTool::setCurrentPage(QString page)
{
    l_currentPage = page;
    emit currentPageChanged();
}

// Save current page to file
void NotesTool::saveCurrentPageContent(QString content)
{
    QFile f(sManager->getSetting(Setting::notesPath) + "/" + l_currentChapter + "/" + l_currentPage + ".txt");

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

    QFile f(sManager->getSetting(Setting::notesPath) + "/" + l_currentChapter + "/" + l_currentPage + ".txt");

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
        QDir d(sManager->getSetting(Setting::notesPath) + "/" + chapter);

        if (!d.exists())
        {
            d.mkpath(d.absolutePath());

            updateChapters();
            emit chaptersChanged();

            l_currentChapter = chapter;
            emit currentChapterChanged();
        }
    }
}

void NotesTool::addPage(QString page)
{
    if (page.length() > 0)
    {
        QFile f(sManager->getSetting(Setting::notesPath) + "/" + l_currentChapter + "/" + page + ".txt");

        if (!f.exists())
        {
            f.open(QIODevice::WriteOnly);
            f.close();

            updatePages();
            emit pagesChanged();

            //            l_currentPage = page;
            emit currentPageChanged();
        }
    }
}

void NotesTool::deleteChapter(QString chapter)
{
    QDir d(sManager->getSetting(Setting::notesPath) + "/" + chapter);

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

        if (l_chapters.size() > 0) l_currentChapter = l_chapters.at(0);
        else l_currentChapter = "";

        emit currentChapterChanged();
    }
}

void NotesTool::deletePage(QString page)
{
    QFile f(sManager->getSetting(Setting::notesPath) + "/" + l_currentChapter + "/" + page + ".txt");

    if (f.exists())
    {
        f.remove();

        updatePages();
        emit pagesChanged();

        if (l_pages.size() > 0) l_currentPage = l_pages.at(0);
        else l_currentPage = "";
        emit currentPageChanged();
    }
}

// Returns content ROT 13 encrypted
QString NotesTool::encrypt(QString content)
{
    return rot13(content);
}
