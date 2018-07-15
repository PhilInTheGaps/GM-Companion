#ifndef NOTESTOOL_H
#define NOTESTOOL_H

#include "gm-companion/settings/settingsmanager.h"

#include <QObject>
#include <QStringList>

class NotesTool : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList chapters READ chapters NOTIFY chaptersChanged)
    Q_PROPERTY(QStringList pages READ pages NOTIFY pagesChanged)
    Q_PROPERTY(QString currentChapter READ currentChapter WRITE setCurrentChapter NOTIFY currentChapterChanged)
    Q_PROPERTY(QString currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(QString currentPageContent READ currentPageContent)

public:
    explicit NotesTool(QObject *parent = nullptr);

    QStringList chapters();
    QStringList pages();

    Q_INVOKABLE void updateChapters();
    Q_INVOKABLE void updatePages();

    QString currentChapter();
    QString currentPage();

    Q_INVOKABLE void setCurrentChapter(QString chapter);
    Q_INVOKABLE void setCurrentPage(QString page);

    QString currentPageContent();
    Q_INVOKABLE void saveCurrentPageContent(QString content);

    Q_INVOKABLE void addChapter(QString chapter);
    Q_INVOKABLE void addPage(QString page);

    Q_INVOKABLE void deleteChapter(QString chapter);
    Q_INVOKABLE void deletePage(QString page);

    Q_INVOKABLE QString encrypt(QString content);

    Q_INVOKABLE int getCurrentChapterIndex() const { return l_chapters.indexOf(l_currentChapter); }

signals:
    void chaptersChanged();
    void pagesChanged();
    void currentChapterChanged();
    void currentPageChanged();

private:
    SettingsManager *sManager;

    QStringList l_chapters;
    QStringList l_pages;
    QString l_currentChapter;
    QString l_currentPage;
};

#endif // NOTESTOOL_H
