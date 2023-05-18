#ifndef NOTEBOOKPAGE_H
#define NOTEBOOKPAGE_H

#include "models/treeitem.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class NoteBookPage : public TreeItem
{
    Q_OBJECT
public:
    explicit NoteBookPage(const QString &name, int depth, QObject *chapter = nullptr);

    [[nodiscard]] auto content() -> QString;
    void setContent(const QString &content, bool edited = true);

    [[nodiscard]] auto html() const -> QString
    {
        return m_html;
    }
    [[nodiscard]] auto origContent() const -> QString
    {
        return m_origContent;
    }

    Q_PROPERTY(bool isSaved READ isSaved WRITE setIsSaved NOTIFY isSavedChanged)
    [[nodiscard]] auto isSaved() const -> bool
    {
        return m_isSaved;
    }
    void setIsSaved(bool isSaved);

    AUTO_PROPERTY_VAL2(bool, isCurrent, false)
    AUTO_PROPERTY_VAL2(bool, wasEdited, false)
    AUTO_PROPERTY_VAL2(int, cursorPosition, 0)

public slots:
    void onContentLoaded(const QString &content);
    void onHtmlGenerated(const QString &html, int id);
    void save();
    void close();
    void rename(const QString &newName) override;
    void remove() override
    {
        emit deletePage();
    }

signals:
    void loadPageContent();
    void contentLoaded();
    void generateHtml(const QString &raw, int id);
    void htmlGenerated();
    void isSavedChanged();
    void savePage();
    void closePage();
    void renamePage(const QString &oldPath);
    void deletePage();

private:
    QString m_origContent;
    QString m_content;
    QString m_html;

    bool m_wasContentLoaded = false;
    bool m_isSaved = true;

    static int editCount;
    int m_editCount = 0;
};

#endif // NOTEBOOKPAGE_H
