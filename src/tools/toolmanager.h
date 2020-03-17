#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "managers/filemanager.h"
#include "audio/audiotool.h"
#include "maps/maptool.h"
#include "dicetool.h"
#include "shop/shoptool.h"
#include "characters/charactertool.h"
#include "combat_tracker/combattracker.h"
#include "generators/namegenerator.h"
#include "notestool.h"
#include "convertertool.h"
#include "settings/settingstool.h"

class ToolManager : public QObject
{
    Q_OBJECT
public:
    explicit ToolManager(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent = nullptr);
    ~ToolManager();

    AudioTool* getAudioTool() const { return audioTool; }
    MapTool* getMapTool() const { return mapTool; }
    DiceTool* getDiceTool() const { return diceTool; }
    ShopTool* getShopTool() const { return shopTool; }
    CombatTracker* getCombatTracker() const { return combatTracker; }
    CharacterTool* getCharacterTool() const { return characterTool; }
    NameGenerator* getNameGenerator() const { return nameGenerator; }
    NotesTool* getNotesTool() const { return notesTool; }
    ConverterTool* getConverterTool() const { return converterTool; }
    SettingsTool* getSettingsTool() const { return settingsTool; }

private:
    FileManager *fileManager;
    QQmlApplicationEngine *qmlEngine;

    AudioTool *audioTool;
    MapTool *mapTool;
    DiceTool *diceTool;
    ShopTool *shopTool;
    CombatTracker *combatTracker;
    CharacterTool *characterTool;
    NameGenerator *nameGenerator;
    NotesTool *notesTool;
    ConverterTool *converterTool;
    SettingsTool *settingsTool;

};

#endif // TOOLMANAGER_H
