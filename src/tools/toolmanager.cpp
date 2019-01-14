#include "toolmanager.h"

#include <QDebug>

ToolManager::ToolManager(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent) : QObject(parent), fileManager(fManager), qmlEngine(engine)
{
    qDebug() << "Starting Tool Manager ...";

    audioTool     = new AudioTool(fManager, qmlEngine, this);
    mapTool       = new MapTool(fManager, qmlEngine, this);
    diceTool      = new DiceTool;
    combatTracker = new CombatTracker(qmlEngine);
    shopTool      = new ShopTool;
    characterTool = new CharacterTool;
    nameGenerator = new NameGenerator;
    notesTool     = new NotesTool();
    converterTool = new ConverterTool();
    settingsTool  = new SettingsTool();
}

ToolManager::~ToolManager()
{
    delete audioTool;
    delete mapTool;
    delete diceTool;
    delete shopTool;
    delete combatTracker;
    delete notesTool;
    delete converterTool;
    delete settingsTool;
}
