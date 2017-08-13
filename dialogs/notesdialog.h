#ifndef NOTESDIALOG_H
#define NOTESDIALOG_H

#include <QDialog>

#include <mainwindow.h>
#include "managers/settingsmanager.h"
#include "tools/notestool.h"

namespace Ui {
    class NotesDialog;
}

class NotesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotesDialog(NotesTool *parent = 0);
    ~NotesDialog();

    void setFolder(QString f);

    void setPage(int i);

private slots:
    void on_createNoteDialog_accepted();

    void on_createNoteDialog_rejected();

    void on_errorNoteDialog_rejected();

    void on_addCategoryDialog_accepted();

    void on_addCategoryDialog_rejected();

private:
    Ui::NotesDialog *ui;

    NotesTool *w;

    SettingsManager *settingsManager;

    QString folder;
    QString filename;
};

#endif // NOTESDIALOG_H
