#include "settings/settingsmanager.h"
#include "logging.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QTranslator>
#include <QFontDatabase>
#include <QIcon>
#include <QSGRendererInterface>

#include "tools/audio/audiotool.h"
#include "tools/maps/maptool.h"
#include "tools/dicetool.h"
#include "tools/combat_tracker/combattracker.h"
#include "tools/shop/shoptool.h"
#include "tools/characters/charactertool.h"
#include "tools/generators/namegenerator.h"
#include "tools/notestool.h"
#include "tools/convertertool.h"
#include "filesystem/filemanager.h"
#include "filesystem/filedialog.h"

#include "services/spotify.h"
#include "services/googledrive.h"

#include "tools/project_converter/projectconverter.h"

#include "managers/addonmanager.h"
#include "managers/updatemanager.h"
#include "platformdetails.h"

/// Register meta types for signals and slots
void registerMetaTypes()
{
    qmlRegisterType<FileDialog>("lol.rophil.gmcompanion.filedialog", 1, 0, "FileDialogBackend");

    qRegisterMetaType<AudioElement *>();
    qRegisterMetaType<QList<AudioProject *> >();
    qRegisterMetaType<QList<QNetworkReply::RawHeaderPair> >();
}

/// Set the language and install a translator
void initTranslations()
{
    qCDebug(gmMain()) << "Initializing translations ...";
    auto *translator = new QTranslator;

    if (translator->load(SettingsManager::getLanguage(), "gm-companion", "_", ":/translations"))
    {
        QGuiApplication::installTranslator(translator);

        qCDebug(gmMain()) << "Loaded translation" << SettingsManager::getLanguage();
    }
    else
    {
        qCWarning(gmMain) << "Warning: Could not load translation";
    }
}

/// Install fonts for FontAwesome.pri
void installFonts()
{
    QFontDatabase::addApplicationFont(":/fonts/fa-solid.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fa-regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fa-brands.ttf");
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName("GM-Companion");
    QGuiApplication::setOrganizationName("GM-Companion");
    QGuiApplication::setOrganizationDomain("gm-companion.github.io");
    QGuiApplication::setWindowIcon(QIcon(":/icons/gm-companion/icon.png"));

#if defined(Q_OS_WIN)
    app.setFont(QFont("Segoe UI"));
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::OpenGL);
#endif // if defined(Q_OS_WIN)

    qCDebug(gmMain()) << "Starting GM-Companion ...";

    registerMetaTypes();
    initTranslations();
    installFonts();

    // Convert Projects to newest version
    ProjectConverter projectConverter;
    projectConverter.convert();

    // Make classes available for QML
    QUrl source(QStringLiteral("qrc:/main.qml"));

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");

    // Load tools
    AudioTool audioTool(&engine);
    MapTool   mapTool(&engine);
    DiceTool  diceTool(&engine);
    CombatTracker combatTracker(&engine);
    ShopTool shopTool(&engine);
    CharacterTool characterTool(&engine);
    NameGenerator nameGenerator(&engine);
    NotesTool     notesTool(&engine);
    ConverterTool converterTool(&engine);

    // Misc
    engine.rootContext()->setContextProperty("file_manager", FileManager::getInstance());
    engine.rootContext()->setContextProperty("settings_manager", SettingsManager::getInstance());
    engine.rootContext()->setContextProperty("update_manager", new UpdateManager);
    engine.rootContext()->setContextProperty("platform", new PlatformDetails);

    //    engine.rootContext()->setContextProperty("addon_manager", new
    // AddonManager);

    // Services
    engine.rootContext()->setContextProperty("spotify_service", Spotify::getInstance());
    engine.rootContext()->setContextProperty("googledrive_service", GoogleDrive::getInstance());

    engine.load(source);

    if (engine.rootObjects().isEmpty()) return -1;

    return QGuiApplication::exec();
}
