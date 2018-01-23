#include "notestool.h"
#include "ui_notestool.h"
#include "gm-companion/functions.h"
#include "gm-companion/tools/notes/notesdialog.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTreeWidgetItem>
#include <QScrollBar>
#include <QTextStream>

NotesTool::NotesTool(QWidget *parent) : QWidget(parent), ui(new Ui::NotesTool)
{
    qDebug() << "Loading NotesTool ...";

    ui->setupUi(this);

    settingsManager = new SettingsManager;

    getNotes();

    ui->textEdit_notes->setFontPointSize(ui->spinBox_fontSize->value());
}

NotesTool::~NotesTool()
{
    delete ui;
}

// Encrypt Note
void NotesTool::on_pushButton_encrypt_clicked()
{
    QString content = ui->textEdit_notes->toPlainText();

    content = rot13(content);

    ui->textEdit_notes->setText(content);
    ui->textEdit_notes->verticalScrollBar()->setValue(0);
}

void NotesTool::getNotes()
{
    qDebug() << "Getting Notes...";

    ui->treeWidget_notes->clear();
    QString notesFolder = settingsManager->getSetting(Setting::notesPath);

    qDebug() << "Getting Custom Notes...";

    // Notes inside notes directory
    for (QString category : getFolders(notesFolder))
    {
        if (!category.contains("."))
        {
            QTreeWidgetItem* ti = new QTreeWidgetItem(0);
            ti->setText(0, category);

            ui->treeWidget_notes->addTopLevelItem(ti);

            for (QString notes : getFiles(notesFolder+"/"+category))
            {
                if (notes.contains(".txt"))
                {
                    QTreeWidgetItem* i = new QTreeWidgetItem(1);
                    i->setText(0, cleanText(notes));

                    ti->addChild(i);
                }
            }

            if (category == currentCategory)
                ti->setExpanded(true);
        }
    }

    qDebug() << "Getting Addon Notes...";

    // Notes inside addons
    QString addonsFolder = QDir::homePath()+"/.gm-companion/addons";
    for (QString addon : getFolders(addonsFolder))
    {
        if (!addon.contains("."))
        {
            if (settingsManager->getIsAddonEnabled(addon) && QDir(addonsFolder+"/"+addon+"/notes").exists())
            {
                QTreeWidgetItem* ti = new QTreeWidgetItem(2);
                ti->setText(0, addon);

                ui->treeWidget_notes->addTopLevelItem(ti);

                for (QString notes : getFiles(addonsFolder+"/"+addon+"/notes"))
                {
                    if (notes.contains(".txt"))
                    {
                        QTreeWidgetItem* i = new QTreeWidgetItem(3);
                        i->setText(0, cleanText(notes));

                        ti->addChild(i);
                    }
                }
            }
        }
    }

    qDebug() << "Finished getting Notes.";
}

void NotesTool::readNotes(QString file)
{
    QFile f(file);
    if (f.exists())
    {
        f.open(QFile::ReadOnly);
        QString content = QString::fromUtf8(f.readAll());
        ui->textEdit_notes->setText(content);
        ui->textEdit_notes->verticalScrollBar()->setValue(0);
        f.close();
    }
}

void NotesTool::addNotes()
{
    if (ui->treeWidget_notes->currentItem() != NULL)
    {
        switch (ui->treeWidget_notes->currentItem()->type())
        {
        case 0: // Category
        {
            NotesDialog* d = new NotesDialog(this);
            d->setFolder(ui->treeWidget_notes->currentItem()->text(0));
            d->setPage(0);
            d->show();
            break;
        }
        case 1: // Notes
        {
            NotesDialog* d = new NotesDialog(this);
            QString category = ui->treeWidget_notes->currentItem()->parent()->text(0);
            currentCategory = category;

            d->setFolder(category);
            d->setPage(0);
            d->show();
            break;
        }
        case 2: // Addon Category
        {
            NotesDialog* d = new NotesDialog(this);
            d->setPage(1);
            d->show();
            break;
        }
        case 3: // Addon Notes
        {
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

void NotesTool::addCategory()
{
    NotesDialog* d = new NotesDialog(this);
    d->setPage(2);
    d->show();
}

void NotesTool::deleteNotes()
{
    if (ui->treeWidget_notes->currentItem() != NULL)
    {
        if (ui->treeWidget_notes->currentItem()->type() == 1)
        {
            QString folder = ui->treeWidget_notes->currentItem()->parent()->text(0);
            QString path = settingsManager->getSetting(Setting::notesPath)+"/"+folder+"/"+ui->treeWidget_notes->currentItem()->text(0)+".txt";
            QFile file(path);

            qDebug().noquote() << "Removing Note:" << path;

            file.remove();
            getNotes();
        }
    }
}

void NotesTool::deleteCategory()
{
    if (ui->treeWidget_notes->currentItem() != NULL)
    {
        if (ui->treeWidget_notes->currentItem()->type() == 0)
        {
            QString path = settingsManager->getSetting(Setting::notesPath)+"/"+ui->treeWidget_notes->currentItem()->text(0);

            qDebug().noquote() << "Removing Category:" << path;

            QDir().rmdir(path);

            getNotes();
        }
    }
}

void NotesTool::on_treeWidget_notes_itemClicked(QTreeWidgetItem *item, int column)
{
    switch (item->type()) {
    case 0: // Normal Categories
        currentCategory = item->text(column);
        break;
    case 1: // Normal Notes
        ui->textEdit_notes->setReadOnly(false);
        readNotes(settingsManager->getSetting(Setting::notesPath)+"/"+item->parent()->text(column)+"/"+item->text(column)+".txt");
        currentCategory = item->parent()->text(column);
        break;
    case 3: // Addon Notes
        ui->textEdit_notes->setReadOnly(false);
        readNotes(QDir::homePath()+"/.gm-companion/addons/"+item->parent()->text(column)+"/notes/"+item->text(column)+".txt");
        break;
    default:
        break;
    }
}

void NotesTool::on_textEdit_notes_textChanged()
{
    int column = ui->treeWidget_notes->currentColumn();

    if (column > -1) {

        QString filename = ui->treeWidget_notes->currentItem()->text(column)+".txt";
        QString parent = ui->treeWidget_notes->currentItem()->parent()->text(column);

        if (ui->treeWidget_notes->currentItem()->type() == 1)
        {
            QFile file(settingsManager->getSetting(Setting::notesPath)+"/"+parent+"/"+filename);

            if (file.exists())
            {
                file.open(QFile::WriteOnly);
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                QString output = ui->textEdit_notes->document()->toPlainText().toUtf8();
                stream << output;
                file.close();
            }
        }
        else if (ui->treeWidget_notes->currentItem()->type() == 3)
        {
            QFile file(QDir::homePath()+"/.gm-companion/addons/"+parent+"/notes/"+filename);

            if (file.exists())
            {
                file.open(QFile::WriteOnly);
                QTextStream stream(&file);
                stream.setCodec("UTF-8");
                QString output = ui->textEdit_notes->document()->toPlainText().toUtf8();
                stream << output;
                file.close();
            }
        }
    }
}

void NotesTool::updateText()
{
    if (!ui->textEdit_notes->toPlainText().isEmpty())
    {
        QString text = ui->textEdit_notes->toPlainText();
        ui->textEdit_notes->setText(text);
    }
}

void NotesTool::on_pushButton_addNote_clicked()
{
    addNotes();
}

void NotesTool::on_pushButton_addCategory_clicked()
{
    addCategory();
}

void NotesTool::on_pushButton_deleteNote_clicked()
{
    deleteNotes();
}

void NotesTool::on_pushButton_deleteCategory_clicked()
{
    deleteCategory();
}

void NotesTool::on_fontComboBox_currentTextChanged(const QString &arg1)
{
    ui->textEdit_notes->setFontFamily(arg1);
    updateText();
}

void NotesTool::on_spinBox_fontSize_valueChanged(int arg1)
{
    ui->textEdit_notes->setFontPointSize(arg1);
    updateText();
}

void NotesTool::on_pushButton_update_clicked()
{
    getNotes();
}
