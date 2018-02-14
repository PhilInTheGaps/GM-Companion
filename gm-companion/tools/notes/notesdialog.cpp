#include "notesdialog.h"
#include "ui_notesdialog.h"
#include "gm-companion/tools/notes/notestool.h"

#include <QFile>
#include <QDir>
#include <QIODevice>
#include <QDebug>

NotesDialog::NotesDialog(NotesTool *parent) : QDialog(parent), ui(new Ui::NotesDialog)
{
    ui->setupUi(this);

    w = parent;

    settingsManager = new SettingsManager;
}

NotesDialog::~NotesDialog()
{
    delete ui;
}

void NotesDialog::setPage(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

void NotesDialog::setFolder(QString f)
{
    folder = f;
}

void NotesDialog::on_createNoteDialog_accepted()
{
    filename = ui->noteNameLine->text() + ".txt";

    QFile file(settingsManager->getSetting(Setting::notesPath) + "/" + folder + "/" + filename);

    if (!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    this->close();
    w->getNotes();
}

void NotesDialog::on_createNoteDialog_rejected()
{
    this->close();
}

void NotesDialog::on_errorNoteDialog_rejected()
{
    this->close();
}

void NotesDialog::on_addCategoryDialog_accepted()
{
    folder = ui->categoryLine->text();

    QDir dir(settingsManager->getSetting(Setting::notesPath) + "/" + folder);

    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    this->close();
    w->getNotes();
}

void NotesDialog::on_addCategoryDialog_rejected()
{
    this->close();
}
