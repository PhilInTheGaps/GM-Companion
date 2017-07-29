#include "deletecharacterdialog.h"
#include "ui_deletecharacterdialog.h"

#include <QFile>

DeleteCharacterDialog::DeleteCharacterDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DeleteCharacterDialog)
{
    ui->setupUi(this);

    ui->charLabel->setText(characterFile);
}

DeleteCharacterDialog::~DeleteCharacterDialog()
{
    delete ui;
}

void DeleteCharacterDialog::setCharacterFile(QString file){
    characterFile = file;
    ui->charLabel->setText(characterFile);
}

void DeleteCharacterDialog::on_buttonBox_accepted()
{
    if (QFile(characterFile).exists())
        QFile::remove(characterFile);
}
