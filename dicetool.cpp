#include "dicetool.h"
#include "ui_dicetool.h"

DiceTool::DiceTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiceTool)
{
    ui->setupUi(this);
}

DiceTool::~DiceTool()
{
    delete ui;
}
