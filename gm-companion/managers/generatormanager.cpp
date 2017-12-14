#include "generatormanager.h"
#include "ui_generatormanager.h"

#include "gm-companion/generators/namegenerator.h"
#include "gm-companion/addontools/sifrp.h"

#include <QDebug>

GeneratorManager::GeneratorManager(QWidget *parent) : QWidget(parent), ui(new Ui::GeneratorManager)
{
    qDebug().noquote() << "Loading GeneratorManager ...";

    ui->setupUi(this);

    settingsManager = new SettingsManager;

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

    // Addons
    qDebug().noquote() << "Loading addon generators ...";
    if (settingsManager->getIsAddonEnabled("SIFRP"))
    {
        qDebug().noquote() << "SIFRP addon is enabled, loading generators ...";
        SIFRP* sifrp = new SIFRP(this);
        ui->tabWidget_generators->addTab(sifrp, "SIFRP");
    }
}
