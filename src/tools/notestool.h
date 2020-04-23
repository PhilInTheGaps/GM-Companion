#ifndef NOTESTOOL_H
#define NOTESTOOL_H

#include "utils/utils.h"

#include <QObject>
#include <QStringList>
#include <QQmlApplicationEngine>

class NotesTool : public QObject
{
    Q_OBJECT

public:
    explicit NotesTool(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    Q_PROPERTY(QStringList chapters READ chapters NOTIFY chaptersChanged)
    Q_PROPERTY(QString currentChapter READ currentChapter WRITE setCurrentChapter NOTIFY currentChapterChanged)
    QStringList chapters();
    QString currentChapter();
    Q_INVOKABLE void setCurrentChapter(QString chapter);
    Q_INVOKABLE void addChapter(QString chapter);
    Q_INVOKABLE void updateChapters();
    Q_INVOKABLE void deleteChapter(QString chapter);
    Q_INVOKABLE int getCurrentChapterIndex() const { return m_chapters.indexOf(m_currentChapter); }

    Q_PROPERTY(QStringList pages READ pages NOTIFY pagesChanged)
    Q_PROPERTY(QString currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    QStringList pages();
    QString currentPage();
    Q_INVOKABLE void setCurrentPage(QString page);
    Q_INVOKABLE void addPage(QString page);
    Q_INVOKABLE void updatePages();
    Q_INVOKABLE void deletePage(QString page);
    Q_INVOKABLE int getCurrentPageIndex() const { return m_pages.indexOf(m_currentPage); }

    Q_PROPERTY(QString currentPageContent READ currentPageContent NOTIFY currentPageChanged)
    QString currentPageContent() const { return m_currentContent; }
    Q_INVOKABLE void loadPageContent();
    Q_INVOKABLE void saveCurrentPageContent(QString content);

    Q_INVOKABLE QString encrypt(QString content) { return Utils::rot13(content); }

signals:
    void chaptersChanged();
    void pagesChanged();
    void currentChapterChanged();
    void currentPageChanged();

private:
    QStringList m_chapters;
    QStringList m_pages;
    QString m_currentChapter;
    QString m_currentPage;
    QString m_currentContent;

    int m_folderListRequestId = -1;
    int m_pageListRequestId = -1;
    int m_contentRequestId = -1;

    void loadChapters(QStringList folders);
    void loadPages(QStringList files);

private slots:
    void onReceivedFileList(int requestId, QStringList files);
    void onReceivedFile(int requestId, QByteArray data);
};

#endif // NOTESTOOL_H
