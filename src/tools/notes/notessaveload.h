#ifndef NOTESSAVELOAD_H
#define NOTESSAVELOAD_H

#include <QObject>
#include <QTextDocument>
#include "notebook.h"

class NotesSaveLoad : public QObject
{
    Q_OBJECT
public:
    explicit NotesSaveLoad(QObject *parent = nullptr);
    void exportPage(NoteBookPage *page, QTextDocument *document);

public slots:
    void loadBooks();
    void createBook(const QString &name, QObject *root);

signals:
    void booksLoaded(QObject* root);
    void pagesLoaded(const QList<NoteBookPage*> &pages);

private:
    void buildBooks(const QStringList& folders, TreeItem *root);
    void buildChapters(const QStringList& folders, NoteBook *book);
    void buildPages(const QStringList& files, NoteBookChapter *chapter);
    NoteBookPage *buildPage(const QString &name, NoteBookChapter *chapter, bool emitSignal = true);

    static QString getPdfPath(NoteBookPage *page);

private slots:
    void loadChapters();
    void loadPages();
    void loadPageContent();
    void savePage();

    void createChapter(const QString& name);
    void createPage(const QString &name);

    void renameChapter(const QString &oldPath);
    void renamePage(const QString &oldPath);

    void deleteChapter();
    void deletePage();
};

#endif // NOTESSAVELOAD_H
