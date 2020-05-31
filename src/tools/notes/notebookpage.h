#ifndef NOTEBOOKPAGE_H
#define NOTEBOOKPAGE_H

#include "models/treeitem.h"

class NoteBookPage : public TreeItem
{
    Q_OBJECT
public:
    explicit NoteBookPage(const QString& name, int depth, QObject *chapter = nullptr);

    QString content();
    void setContent(const QString &content, bool edited = true);

    QString html() const { return m_html; }
    QString origContent() const { return m_origContent; }

    Q_PROPERTY(bool isSaved READ isSaved WRITE setIsSaved NOTIFY isSavedChanged)
    bool isSaved() const { return m_isSaved; }
    void setIsSaved(bool isSaved);

    Q_PROPERTY(bool isCurrent READ isCurrent WRITE setIsCurrent NOTIFY isCurrentChanged)
    bool isCurrent() const { return m_isCurrent; }
    void setIsCurrent(bool isCurrent) { m_isCurrent = isCurrent; emit isCurrentChanged(); }

    Q_PROPERTY(bool wasEdited READ wasEdited WRITE setWasEdited NOTIFY wasEditedChanged)
    bool wasEdited() const { return m_wasEdited; }
    void setWasEdited(bool wasEdited) { m_wasEdited = wasEdited; emit wasEditedChanged(); }

    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    int cursorPosition() const { return m_cursorPosition; }
    void setCursorPosition(int position) { m_cursorPosition = position; emit cursorPositionChanged(); }

public slots:
    void onContentLoaded(const QString &content);
    void onHtmlGenerated(const QString &html, int id);
    void save();
    void close();
    void rename(const QString& newName) override;
    void remove() override { emit deletePage(); }

signals:
    void loadPageContent();
    void contentLoaded();
    void generateHtml(const QString &raw, int id);
    void htmlGenerated();
    void isSavedChanged();
    void isCurrentChanged();
    void wasEditedChanged();
    void cursorPositionChanged();
    void savePage();
    void closePage();
    void renamePage(const QString &oldPath);
    void deletePage();

private:
    QString m_origContent;
    QString m_content;
    QString m_html;

    int m_cursorPosition = 0;

    bool m_wasContentLoaded = false;
    bool m_isSaved = true;
    bool m_isCurrent = false;
    bool m_wasEdited = false;

    static int editCount;
    int m_editCount = 0;

};

#endif // NOTEBOOKPAGE_H
