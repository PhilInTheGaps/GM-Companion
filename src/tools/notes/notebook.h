#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include "notebookchapter.h"

class NoteBook : public NoteBookChapter
{
    Q_OBJECT

public:
    explicit NoteBook(const QString &name, QObject *parent = nullptr);

    auto chapters() -> QList<NoteBookChapter *>;

    [[nodiscard]] auto creatables() const -> QStringList override
    {
        return {tr("Chapter"), tr("Note")};
    }

public slots:
    void onChaptersLoaded();
    void create(const QString &type, const QString &name) override;

signals:
    void loadChapters();
    void createChapter(const QString &name);

private:
    bool m_wereChaptersLoaded = false;
    QList<NoteBookChapter *> m_chapters;

private slots:
    void onIsOpenChanged() override;
};

#endif // NOTEBOOK_H
