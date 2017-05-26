#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
#include "notesdialog.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTreeWidgetItem>
#include <QScrollBar>
#include <QTextStream>

void MainWindow::getNotes(){
    qDebug() << "Getting Notes...";

    ui->notesTree->clear();

    QString notesFolder = QDir::homePath()+"/.gm-companion/notes";

    qDebug() << "Getting Custom Notes...";
    // Notes inside /.gm-companion/notes
    for (QString category : getFolders(notesFolder)){
        if (!category.contains(".")){
            QTreeWidgetItem* ti = new QTreeWidgetItem(0);
            ti->setText(0, category);

            ui->notesTree->addTopLevelItem(ti);

            for (QString notes : getFiles(notesFolder+"/"+category)){
                if (notes.contains(".txt")){
                    QTreeWidgetItem* i = new QTreeWidgetItem(1);
                    i->setText(0, cleanText(notes));

                    ti->addChild(i);
                }
            }
            ti->setExpanded(true);
        }
    }

    qDebug() << "Getting Addon Notes...";
    // Notes inside addons
    QString addonsFolder = QDir::homePath()+"/.gm-companion/addons";
    for (QString addon : getFolders(addonsFolder)){
        if (!addon.contains(".")){
            if (settingsManager->getIsAddonEnabled(addon)){
                QTreeWidgetItem* ti = new QTreeWidgetItem(2);
                ti->setText(0, addon);

                ui->notesTree->addTopLevelItem(ti);

                for (QString notes : getFiles(addonsFolder+"/"+addon+"/notes")){
                    if (notes.contains(".txt")){
                        QTreeWidgetItem* i = new QTreeWidgetItem(3);
                        i->setText(0, cleanText(notes));

                        ti->addChild(i);
                    }
                }

                ti->setExpanded(true);
            }
        }
    }
    qDebug() << "Finished getting Notes.";
}

void MainWindow::readNotes(QString file){
    QFile f(file);
    if (f.exists()){
        f.open(QFile::ReadOnly);
        QString content = f.readAll();
        ui->notesTextEdit->setText(content);
        ui->notesTextEdit->verticalScrollBar()->setValue(0);
        f.close();
    }
}

void MainWindow::addNotes(){
    if (ui->notesTree->currentItem() != NULL){
        switch (ui->notesTree->currentItem()->type()) {
        case 0:{
            NotesDialog* d = new NotesDialog(this);
            d->setFolder(ui->notesTree->currentItem()->text(0));
            d->setPage(0);
            d->show();
            break;
        }
        case 1:{
            NotesDialog* d = new NotesDialog(this);
            d->setFolder(ui->notesTree->currentItem()->parent()->text(0));
            d->setPage(0);
            d->show();
            break;
        }
        case 2:{
            NotesDialog* d = new NotesDialog(this);
            d->setPage(1);
            d->show();
            break;
        }
        case 3:{
            NotesDialog* d = new NotesDialog(this);
            d->setPage(1);
            d->show();
            break;
        }
        default:
            break;
        }
    }
}

void MainWindow::addCategory(){
    NotesDialog* d = new NotesDialog(this);
    d->setPage(2);
    d->show();
}

void MainWindow::deleteNotes(){
    if (ui->notesTree->currentItem() != NULL){
        if (ui->notesTree->currentItem()->type() == 1){
            QString folder = ui->notesTree->currentItem()->parent()->text(0);
            QFile file(QDir::homePath()+"/.gm-companion/notes/"+folder+"/"+ui->notesTree->currentItem()->text(0)+".txt");
            qDebug() << "Removing Note: "+QDir::homePath()+"/.gm-companion/notes/"+folder+"/"+ui->notesTree->currentItem()->text(0)+".txt";
            file.remove();
            getNotes();
        }
    }
}

void MainWindow::deleteCategory(){
    if (ui->notesTree->currentItem() != NULL){
        if (ui->notesTree->currentItem()->type() == 0){
            qDebug() << "Removing Note Directory: "+QDir::homePath()+"/.gm-companion/notes/"+ui->notesTree->currentItem()->text(0);
            QDir().rmdir(QDir::homePath()+"/.gm-companion/notes/"+ui->notesTree->currentItem()->text(0));
            getNotes();
        }
    }
}

void MainWindow::on_notesTree_itemClicked(QTreeWidgetItem *item, int column){
    if (item->type() == 1){
        ui->notesTextEdit->setReadOnly(false);
        readNotes(QDir::homePath()+"/.gm-companion/notes/"+item->parent()->text(column)+"/"+item->text(column)+".txt");
    }else if (item->type() == 3){
        ui->notesTextEdit->setReadOnly(false);
        readNotes(QDir::homePath()+"/.gm-companion/addons/"+item->parent()->text(column)+"/notes/"+item->text(column)+".txt");
    }
}

void MainWindow::on_notesTextEdit_textChanged(){
    int column = ui->notesTree->currentColumn();
    QString filename = ui->notesTree->currentItem()->text(column)+".txt";
    QString parent = ui->notesTree->currentItem()->parent()->text(column);

    if (ui->notesTree->currentItem()->type() == 1){
        QFile file(QDir::homePath()+"/.gm-companion/notes/"+parent+"/"+filename);

        if (file.exists()){
            file.open(QFile::WriteOnly);
            QTextStream stream(&file);
            stream << ui->notesTextEdit->document()->toPlainText();
            file.close();
        }
    }else if (ui->notesTree->currentItem()->type() == 3){
        QFile file(QDir::homePath()+"/.gm-companion/addons/"+parent+"/notes/"+filename);

        if (file.exists()){
            file.open(QFile::WriteOnly);
            QTextStream stream(&file);
            stream << ui->notesTextEdit->document()->toPlainText();
            file.close();
        }
    }
}

void MainWindow::notesWatcher_directoryChanged(){
    getNotes();
}

void MainWindow::on_addNoteButton_clicked(){
    addNotes();
}

void MainWindow::on_addCategoryButton_clicked(){
    addCategory();
}

void MainWindow::on_deleteNoteButton_clicked(){
    deleteNotes();
}

void MainWindow::on_deleteCategoryButton_clicked(){
    deleteCategory();
}