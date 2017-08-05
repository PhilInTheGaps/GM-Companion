#include "charactersheeteditor.h"
#include "ui_charactersheeteditor.h"

#include <QDebug>

CharacterSheetEditor::CharacterSheetEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharacterSheetEditor)
{
    ui->setupUi(this);
}

CharacterSheetEditor::~CharacterSheetEditor()
{
    delete ui;
}
