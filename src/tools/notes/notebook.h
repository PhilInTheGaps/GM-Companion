#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include "models/treeitem.h"
#include "notebookchapter.h"

class NoteBook : public NoteBookChapter
{
    Q_OBJECT

public:
    explicit NoteBook(const QString& name, QObject *parent = nullptr);

    QList<NoteBookChapter*> chapters();

    QStringList creatables() const override { return { tr("Chapter"), tr("Note") }; }

public slots:
    void onChaptersLoaded();
    void create(const QString &type, const QString &name) override;

signals:
    void loadChapters();
    void createChapter(const QString &name);

private:
    bool m_wereChaptersLoaded = false;
    QList<NoteBookChapter*> m_chapters;

private slots:
    void onIsOpenChanged() override;
};

#endif // NOTEBOOK_H
