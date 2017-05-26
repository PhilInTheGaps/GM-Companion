#include "notesdialog.h"
#include "ui_notesdialog.h"

#include <QFile>
#include <QDir>
#include <QIODevice>
#include <QDebug>

NotesDialog::NotesDialog(MainWindow *parent) : QDialog(parent), ui(new Ui::NotesDialog){
    ui->setupUi(this);

    w = parent;
}

NotesDialog::~NotesDialog(){
    delete ui;
}

void NotesDialog::setPage(int i){
    ui->stackedWidget->setCurrentIndex(i);
}

void NotesDialog::setFolder(QString f){
    folder = f;
}

void NotesDialog::on_createNoteDialog_accepted(){
    filename = ui->noteNameLine->text()+".txt";

    QFile file(QDir::homePath()+"/.gm-companion/notes/"+folder+"/"+filename);
    if (!file.exists()){
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    this->close();
    w->getNotes();
}

void NotesDialog::on_createNoteDialog_rejected(){
    this->close();
}

void NotesDialog::on_errorNoteDialog_rejected(){
    this->close();
}

void NotesDialog::on_addCategoryDialog_accepted(){
    folder = ui->categoryLine->text();

    QDir dir(QDir::homePath()+"/.gm-companion/notes/"+folder);
    if (!dir.exists()){
        dir.mkpath(".");
    }

    this->close();
    w->getNotes();
}

void NotesDialog::on_addCategoryDialog_rejected(){
    this->close();
}
