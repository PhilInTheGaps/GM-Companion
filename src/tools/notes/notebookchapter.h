#pragma once

#include "models/treeitem.h"
#include "notebookpage.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

class NoteBookChapter : public TreeItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit NoteBookChapter(const QString &name, QObject *parent = nullptr);
    explicit NoteBookChapter(const QString &name, int depth, QObject *parent = nullptr);

    [[nodiscard]] auto pages() const -> QList<NoteBookPage *>;
    [[nodiscard]] auto creatables() const -> QStringList override
    {
        return {tr("Note")};
    }

public slots:
    void onPagesLoaded();
    void create(const QString &type, const QString &name) override;
    void rename(const QString &newName) override;
    void remove() override
    {
        emit deleteChapter();
    }

signals:
    void loadPages();
    void createPage(const QString &name);
    void renameChapter(const QString &oldName);
    void deleteChapter();

protected:
    bool m_werePagesLoaded = false;

protected slots:
    virtual void onIsOpenChanged();
};
