#include "namegenerator.h"
#include "ui_namegenerator.h"

#include <QDebug>

NameGenerator::NameGenerator(QWidget *parent) : QWidget(parent), ui(new Ui::NameGenerator)
{
    qDebug().noquote() << "Loading NameGenerator ...";

    ui->setupUi(this);
}

NameGenerator::~NameGenerator()
{
    delete ui;
}
