#ifndef NOTESTOOL_H
#define NOTESTOOL_H

#include "tools/abstracttool.h"
#include "notessaveload.h"
#include "htmlgenerator.h"
#include <QQmlApplicationEngine>
#include <QQuickTextDocument>

class NotesTool : public AbstractTool
{
    Q_OBJECT
public:
    explicit NotesTool(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    Q_PROPERTY(QObject* notesModel READ notesModel WRITE setNotesModel NOTIFY notesModelChanged)
    QObject* notesModel() const { return m_notesModel; }
    void setNotesModel(QObject* model) { m_notesModel = model; emit notesModelChanged(); }

    Q_PROPERTY(QQuickTextDocument* qmlTextDoc READ qmlTextDoc WRITE setQmlTextDoc NOTIFY qmlTextDocChanged)
    QQuickTextDocument* qmlTextDoc() const { return m_qmlTextDoc; }
    void setQmlTextDoc(QQuickTextDocument *qmlTextDoc);

    Q_PROPERTY(QObject* currentPage READ currentPage NOTIFY currentPageChanged)
    QObject* currentPage() const { return qobject_cast<QObject*>(m_currentPage); }
    void setCurrentPage(NoteBookPage *page);

    Q_PROPERTY(bool editMode READ editMode WRITE setEditMode NOTIFY editModeChanged)
    bool editMode() const { return m_editMode; }
    void setEditMode(bool editMode);

    Q_PROPERTY(QList<QObject*> openedPages READ openedPages NOTIFY openedPagesChanged)
    QList<QObject*> openedPages() const { return m_openedPages; }

public slots:
    void loadData() override;
    void encrypt();
    void exportPdf();
    void linkClicked(const QString &link);
    void createBook(const QString &name) { if (!doesBookExist(name)) m_saveLoad->createBook(name, notesModel()); }

signals:
    void loadBooks();
    void notesModelChanged();
    void qmlTextDocChanged();
    void currentPageChanged();
    void editModeChanged();
    void openedPagesChanged();
    void setCursorPosition(int position);

private:
    NotesSaveLoad *m_saveLoad = nullptr;
    NoteBookPage *m_currentPage = nullptr;
    QQuickTextDocument *m_qmlTextDoc = nullptr;
    HtmlGenerator *m_htmlGenerator = nullptr;

    QObject* m_notesModel = nullptr;
    QList<QObject*> m_openedPages;
    bool m_editMode = false;
    int m_cursorPosition = 0;

    void closeUnneededPages();
    void displayPageContent();
    bool doesBookExist(const QString &name);

private slots:
    void onNoteBooksLoaded(QObject *root) { setNotesModel(root); }
    void onPagesLoaded(const QList<NoteBookPage*> &pages);
    void onPageClicked();
    void onPageContentLoaded();
    void onPageHtmlLoaded();
    void onDocumentEdited();
    void onClosePage();
};

#endif // NOTESTOOL_H
