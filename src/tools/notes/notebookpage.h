#pragma once

#include "models/treeitem.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

class NoteBookPage : public TreeItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit NoteBookPage(const QString &name, int depth, QObject *parent);

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

    Q_INVOKABLE void save();
    Q_INVOKABLE void close();
    Q_INVOKABLE void rename(const QString &newName) override;
    Q_INVOKABLE void remove() override
    {
        emit deletePage();
    }

    AUTO_PROPERTY_VAL2(bool, isCurrent, false)
    AUTO_PROPERTY_VAL2(bool, keepOpen, false)
    AUTO_PROPERTY_VAL2(int, cursorPosition, 0)

public slots:
    void onContentLoaded(const QString &content);

signals:
    void loadPageContent();
    void contentLoaded();
    void htmlGenerated();
    void isSavedChanged();
    void savePage();
    void closePage();
    void renamePage(const QString &oldPath);
    void deletePage();

private:
    void generateHtml();

    QString m_origContent;
    QString m_content;
    QString m_html;

    bool m_wasContentLoaded = false;
    bool m_isSaved = true;

    static int editCount;
    int m_editCount = 0;
};

Q_DECLARE_METATYPE(NoteBookPage *)
