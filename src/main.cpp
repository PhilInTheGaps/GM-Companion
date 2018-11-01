#include "settings/settingsmanager.h"

#include <QTranslator>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QMap>

#include "tools/audio/audiotool.h"
#include "tools/audio/audioeditor.h"
#include "tools/audio/audioexporter.h"
#include "tools/audio/audioeditorfilebrowser.h"
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

#include "settings/settingstool.h"
#include "managers/addonmanager.h"
#include "managers/updatemanager.h"
#include "managers/filemanager.h"
#include "platformdetails.h"
#include "ui/colorscheme.h"
#include "tools/project_converter/projectconverter.h"

void loadQmlClasses()
{
    qmlRegisterType<AudioTool>(             "gm.companion.audiotool",              1, 0, "AudioTool");
    qmlRegisterType<AudioEditor>(           "gm.companion.audioeditor",            1, 0, "AudioEditor");
    qmlRegisterType<AudioExporter>(         "gm.companion.audioexporter",          1, 0, "AudioExporter");
    qmlRegisterType<AudioEditorFileBrowser>("gm.companion.audioeditorfilebrowser", 1, 0, "AudioEditorFileBrowserTool");

    qmlRegisterType<MapTool>(               "gm.companion.maptool",                1, 0, "MapTool");
    qmlRegisterType<DiceTool>(              "gm.companion.dicetool",               1, 0, "DiceTool");
    qmlRegisterType<ShopTool>(              "gm.companion.shoptool",               1, 0, "ShopTool");
    qmlRegisterType<ShopEditor>(            "gm.companion.shopeditor",             1, 0, "ShopEditorTool");
    qmlRegisterType<ItemEditor>(            "gm.companion.itemeditor",             1, 0, "ItemEditorTool");
    qmlRegisterType<CombatTracker>(         "gm.companion.combattracker",          1, 0, "CombatTrackerTool");
    qmlRegisterType<NotesTool>(             "gm.companion.notestool",              1, 0, "NotesTool");
    qmlRegisterType<ConverterTool>(         "gm.companion.convertertool",          1, 0, "ConverterTool");
    qmlRegisterType<NameGenerator>(         "gm.companion.namegeneratortool",      1, 0, "NameGeneratorTool");

    qmlRegisterType<SettingsTool>(          "gm.companion.settingstool",           1, 0, "SettingsTool");
    qmlRegisterType<AddonManager>(          "gm.companion.addonmanager",           1, 0, "AddonManager");

    qmlRegisterType<CharacterTool>(         "gm.companion.charactertool",          1, 0, "CharacterTool");

    qmlRegisterType<UpdateManager>(         "gm.companion.updatemanager",          1, 0, "UpdateManager");
    qmlRegisterType<PlatformDetails>(       "gm.companion.platforms",              1, 0, "PlatformDetails");
    qmlRegisterType<ColorScheme>(           "gm.companion.colorscheme",            1, 0, "ColorScheme");
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

    if (translator->load("gm-companion_" + settingsManager->getSetting(Setting::language), ":/translations")) app.installTranslator(translator);
    else qDebug() << "Could not load translation ...";

    QFontDatabase::addApplicationFont(":/fonts/fa-solid.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fa-regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fa-brands.ttf");

    // Create program files and remove old ones that are no longer required
    FileManager fileManager;
    fileManager.run();

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
}
