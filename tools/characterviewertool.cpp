#include "characterviewertool.h"
#include "ui_characterviewertool.h"

CharacterViewerTool::CharacterViewerTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharacterViewerTool)
{
    ui->setupUi(this);
}

CharacterViewerTool::~CharacterViewerTool()
{
    delete ui;
}
