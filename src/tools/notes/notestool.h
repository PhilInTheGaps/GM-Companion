#pragma once

#include "common/abstracttool.h"
#include "markdownhighlighter.h"
#include "notessaveload.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJSEngine>
#include <QQmlEngine>
#include <QQuickTextDocument>

class NotesTool : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    NotesTool() = delete;
    explicit NotesTool(QObject *parent = nullptr);

    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> NotesTool *;

    Q_PROPERTY(QQuickTextDocument *qmlTextDoc READ qmlTextDoc WRITE setQmlTextDoc NOTIFY qmlTextDocChanged)
    [[nodiscard]] auto qmlTextDoc() const -> QQuickTextDocument *
    {
        return m_qmlTextDoc;
    }
    void setQmlTextDoc(QQuickTextDocument *qmlTextDoc);

    Q_PROPERTY(NoteBookPage *currentPage READ currentPage NOTIFY currentPageChanged)
    [[nodiscard]] auto currentPage() const -> NoteBookPage *
    {
        return m_currentPage;
    }
    void setCurrentPage(NoteBookPage *page);

    Q_PROPERTY(bool editMode READ editMode WRITE setEditMode NOTIFY editModeChanged)
    [[nodiscard]] auto editMode() const -> bool
    {
        return m_editMode;
    }
    void setEditMode(bool editMode);

    AUTO_PROPERTY_VAL2(TreeItem *, notesModel, nullptr)
    READ_LIST_PROPERTY(NoteBookPage, openedPages)

public slots:
    void loadData() override;
    void encrypt();
    void exportPdf();
    void linkClicked(const QString &link) const;
    void createBook(const QString &name);

signals:
    void loadBooks();
    void qmlTextDocChanged();
    void currentPageChanged();
    void editModeChanged();
    void setCursorPosition(int position);

private:
    NotesSaveLoad m_saveLoad;
    NoteBookPage *m_currentPage = nullptr;
    QQuickTextDocument *m_qmlTextDoc = nullptr;
    MarkdownHighlighter m_markdownHighlighter;

    bool m_editMode = false;
    int m_cursorPosition = 0;

    void closeUnneededPages();
    void displayPageContent();

    [[nodiscard]] auto doesBookExist(const QString &name) const -> bool;

private slots:
    void onNoteBooksLoaded(TreeItem *root);
    void onPagesLoaded(const QList<NoteBookPage *> &pages);
    void onPageClicked();
    void onPageContentLoaded();
    void onPageHtmlLoaded();
    void onDocumentEdited();
    void onClosePage();
};
