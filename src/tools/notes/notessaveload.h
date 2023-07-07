#ifndef NOTESSAVELOAD_H
#define NOTESSAVELOAD_H

#include "notebook.h"
#include <QObject>
#include <QTextDocument>

class NotesSaveLoad : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    void exportPage(NoteBookPage *page, QTextDocument *document);

public slots:
    void loadBooks();
    void createBook(const QString &name, QObject *root);

signals:
    void booksLoaded(QObject *root);
    void pagesLoaded(const QList<NoteBookPage *> &pages);

private:
    void buildBooks(const QStringList &folders, TreeItem *root);
    void buildChapters(const QStringList &folders, NoteBook *book) const;
    void buildPages(const QStringList &files, NoteBookChapter *chapter);
    auto buildPage(const QString &name, NoteBookChapter *chapter, bool emitSignal = true) -> NoteBookPage *;

    static auto getPdfPath(NoteBookPage *page) -> QString;

private slots:
    void loadChapters();
    void loadPages();
    void loadPageContent();
    void savePage() const;

    void createChapter(const QString &name);
    void createPage(const QString &name);

    void renameChapter(const QString &oldPath) const;
    void renamePage(const QString &oldPath) const;

    void deleteChapter() const;
    void deletePage() const;
};

#endif // NOTESSAVELOAD_H
