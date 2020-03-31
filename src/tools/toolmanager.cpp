#include "toolmanager.h"

#include <QDebug>

ToolManager::ToolManager(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent), qmlEngine(engine)
{
    qDebug() << "Starting Tool Manager ...";

    audioTool     = new AudioTool(qmlEngine, this);
    mapTool       = new MapTool(qmlEngine, this);
    diceTool      = new DiceTool;
    combatTracker = new CombatTracker(qmlEngine);
    shopTool      = new ShopTool(qmlEngine, this);
    characterTool = new CharacterTool(qmlEngine, this);
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
