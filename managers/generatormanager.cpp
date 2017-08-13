#include "generatormanager.h"
#include "ui_generatormanager.h"

#include "generators/namegenerator.h"

#include <QDebug>

GeneratorManager::GeneratorManager(QWidget *parent) : QWidget(parent), ui(new Ui::GeneratorManager)
{
    qDebug().noquote() << "Loading GeneratorManager ...";

    ui->setupUi(this);

    loadGenerators();
}

GeneratorManager::~GeneratorManager()
{
    delete ui;
}

void GeneratorManager::loadGenerators()
{
    qDebug().noquote() << "Loading generators ...";

    // Name Generator
    NameGenerator *nameGenerator = new NameGenerator;
    ui->tabWidget_generators->addTab(nameGenerator, "Name Generator");
}
