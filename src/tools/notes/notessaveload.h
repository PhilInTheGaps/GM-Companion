#pragma once

#include "notebook.h"
#include <QObject>
#include <QTextDocument>

class NotesSaveLoad : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    static void exportPage(NoteBookPage &page, const QTextDocument &document);

public slots:
    void loadBooks();
    void createBook(const QString &name, TreeItem *root);

signals:
    void booksLoaded(TreeItem *root);
    void pagesLoaded(const QList<NoteBookPage *> &pages);

private:
    void buildBooks(const QStringList &folders, TreeItem *root);
    void buildChapters(const QStringList &folders, NoteBook &book) const;
    void buildPages(const QStringList &files, NoteBookChapter &chapter);
    auto buildPage(const QString &name, NoteBookChapter &chapter, bool emitSignal = true) -> NoteBookPage *;

    static auto getPdfPath(const NoteBookPage &page) -> QString;

private slots:
    void loadChapters();
    void loadPages();
    void loadPageContent();
    void savePage();

    void createChapter(const QString &name);
    void createPage(const QString &name);

    void renameChapter(const QString &oldPath);
    void renamePage(const QString &oldPath);

    void deleteChapter();
    void deletePage();
};
