#pragma once

#include "notebookchapter.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

class NoteBook : public NoteBookChapter
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

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
