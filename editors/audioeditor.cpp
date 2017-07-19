#include "audioeditor.h"
#include "ui_audioeditor.h"

AudioEditor::AudioEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioEditor)
{
    ui->setupUi(this);
}

AudioEditor::~AudioEditor()
{
    delete ui;
}

void AudioEditor::on_pushButton_newCategory_clicked()
{

}
