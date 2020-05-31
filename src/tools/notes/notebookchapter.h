#ifndef NOTEBOOKCHAPTER_H
#define NOTEBOOKCHAPTER_H

#include "models/treeitem.h"
#include "notebookpage.h"

class NoteBookChapter : public TreeItem
{
    Q_OBJECT
public:
    explicit NoteBookChapter(const QString &name, QObject *parent = nullptr);
    explicit NoteBookChapter(const QString &name, int depth, QObject *parent = nullptr);

    QList<NoteBookPage *> pages() const;
    QStringList creatables() const override { return { tr("Note") }; }

public slots:
    void onPagesLoaded();
    void create(const QString &type, const QString &name) override;
    void rename(const QString& newName) override;
    void remove() override { emit deleteChapter(); }

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

#endif // NOTEBOOKCHAPTER_H
