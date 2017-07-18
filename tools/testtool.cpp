#include "testtool.h"
#include "ui_testtool.h"

TestTool::TestTool(QWidget *parent) : QWidget(parent), ui(new Ui::TestTool)
{
    ui->setupUi(this);
}

TestTool::~TestTool()
{
    delete ui;
}
