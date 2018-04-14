#include "settings/settingsmanager.h"

#include <QTranslator>
#include <QDebug>
#include <QSettings>
#include <QDir>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>

#include "tools/audio/audiotool.h"
#include "tools/maptool.h"
#include "tools/dicetool.h"
#include "tools/shop/shoptool.h"
#include "tools/shop/shopeditor.h"
#include "tools/shop/itemeditor.h"
#include "tools/combattracker.h"
#include "tools/notestool.h"
#include "tools/convertertool.h"
#include "tools/generators/namegenerator.h"

#include "tools/characters/charactertool.h"
#include "tools/characters/charactersaveload.h"
#include "tools/characters/defaultsheet.h"
#include "tools/characters/dsa5sheet.h"

#include "settings/settingstool.h"
#include "managers/updatemanager.h"
#include "platformdetails.h"
#include "ui/colorscheme.h"
#include "tools/project_converter/projectconverter.h"

void loadQmlClasses()
{
    qmlRegisterType<AudioTool>(        "gm.companion.audiotool",         1, 0, "AudioTool");
    qmlRegisterType<MapTool>(          "gm.companion.maptool",           1, 0, "MapTool");
    qmlRegisterType<DiceTool>(         "gm.companion.dicetool",          1, 0, "DiceTool");
    qmlRegisterType<ShopTool>(         "gm.companion.shoptool",          1, 0, "ShopTool");
    qmlRegisterType<ShopEditor>(       "gm.companion.shopeditor",        1, 0, "ShopEditorTool");
    qmlRegisterType<ItemEditor>(       "gm.companion.itemeditor",        1, 0, "ItemEditorTool");
    qmlRegisterType<CombatTracker>(    "gm.companion.combattracker",     1, 0, "CombatTrackerTool");
    qmlRegisterType<NotesTool>(        "gm.companion.notestool",         1, 0, "NotesTool");
    qmlRegisterType<ConverterTool>(    "gm.companion.convertertool",     1, 0, "ConverterTool");
    qmlRegisterType<SettingsTool>(     "gm.companion.settingstool",      1, 0, "SettingsTool");
    qmlRegisterType<NameGenerator>(    "gm.companion.namegeneratortool", 1, 0, "NameGeneratorTool");

    qmlRegisterType<CharacterTool>(    "gm.companion.charactertool",     1, 0, "CharacterTool");
    qmlRegisterType<CharacterSaveLoad>("gm.companion.charactersaveload", 1, 0, "CharacterSaveLoad");
    qmlRegisterType<DefaultSheet>(     "gm.companion.defaultsheet",      1, 0, "DefaultSheetTool");
    qmlRegisterType<DSA5Sheet>(        "gm.companion.dsa5sheet",         1, 0, "DSA5SheetTool");

    qmlRegisterType<UpdateManager>(    "gm.companion.updatemanager",     1, 0, "UpdateManager");
    qmlRegisterType<PlatformDetails>(  "gm.companion.platforms",         1, 0, "PlatformDetails");
    qmlRegisterType<ColorScheme>(      "gm.companion.colorscheme",       1, 0, "ColorScheme");
}

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif // if defined(Q_OS_WIN)

    QGuiApplication app(argc, argv);

    qDebug().noquote() << "Starting GM-Companion ...";

    // Set the language and install a translator
    qDebug().noquote() << "Initializing translations ...";
    SettingsManager *settingsManager = new SettingsManager;
    QTranslator     *translator      = new QTranslator();

    if (translator->load("gm-companion_" + settingsManager->getSetting(language), ":/translations")) app.installTranslator(translator);
    else qDebug() << "Could not load translation ...";

    // Convert Projects to newest version
    ProjectConverter projConverter;
    projConverter.convert();

    // Make classes available for QML
    QUrl source(QStringLiteral("qrc:/main.qml"));

    loadQmlClasses();

    // Set Icon
    app.setWindowIcon(QIcon(":/icons/gm-companion/icon256_new.png"));

    QQmlApplicationEngine engine;

    engine.load(source);

    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();

    /*
        // Update Manager checks if a new version of the gm-companion is
       available
        UpdateManager *updateManager = new UpdateManager(1000);
        updateManager->checkForUpdates();
     */
}
