#include "settings/settingsmanager.h"

#include <QTranslator>
#include <QDebug>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QSGRendererInterface>
#include <QQuickWindow>

#include "tools/toolmanager.h"
#include "services/spotify.h"

#include "settings/settingstool.h"
#include "managers/addonmanager.h"
#include "managers/updatemanager.h"
#include "platformdetails.h"
#include "ui/colorscheme.h"
#include "tools/project_converter/projectconverter.h"
#include "tools/audio/audioelementimageprovider.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName("GM-Companion");
    QGuiApplication::setOrganizationName("GM-Companion");
    QGuiApplication::setOrganizationDomain("gm-companion.github.io");

#if defined(Q_OS_WIN)
    app.setFont(QFont("Segoe UI"));
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::OpenGL);
#endif // if defined(Q_OS_WIN)

    qDebug().noquote() << "Starting GM-Companion ...";

    // Register meta types
    qRegisterMetaType<AudioElement *>();
    qRegisterMetaType<QList<AudioProject *> >();
    qRegisterMetaType<QList<QNetworkReply::RawHeaderPair> >();

    // Set the language and install a translator
    qDebug().noquote() << "Initializing translations ...";
    auto *translator = new QTranslator();

    if (translator->load("gm-companion_" + SettingsManager::getLanguage(), ":/translations")) QGuiApplication::installTranslator(translator);
    else qDebug() << "Could not load translation ...";

    // Install fonts for FontAwesome.pri
    QFontDatabase::addApplicationFont(":/fonts/fa-solid.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fa-regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fa-brands.ttf");

    // Convert Projects to newest version
    ProjectConverter projConverter;
    projConverter.convert();

    // Make classes available for QML
    QUrl source(QStringLiteral("qrc:/main.qml"));

    // Set Window Icon
    QGuiApplication::setWindowIcon(QIcon(":/icons/gm-companion/icon.png"));

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:///");
    engine.addImageProvider("audioElementIcons", new AudioElementImageProvider);

    auto *toolManager = new ToolManager(&engine, nullptr);

    // Load Tools
    engine.rootContext()->setContextProperty("tool_manager", toolManager);

    // Audio
    engine.rootContext()->setContextProperty("audio_tool", toolManager->getAudioTool());
    engine.rootContext()->setContextProperty("audio_editor", toolManager->getAudioTool()->getEditor());
    engine.rootContext()->setContextProperty("audio_exporter", toolManager->getAudioTool()->getEditor()->getAudioExporter());
    engine.rootContext()->setContextProperty("audio_addon_element_manager", toolManager->getAudioTool()->getEditor()->getAddonElementManager());
    engine.rootContext()->setContextProperty("audio_editor_file_browser", toolManager->getAudioTool()->getEditor()->getFileBrowser());

    // Combat Tracker
    engine.rootContext()->setContextProperty("combat_tracker", toolManager->getCombatTracker());
    engine.rootContext()->setContextProperty("combat_tracker_effects", toolManager->getCombatTracker()->getEffectTool());

    // Item Shop
    engine.rootContext()->setContextProperty("shop_tool", toolManager->getShopTool());
    engine.rootContext()->setContextProperty("shop_editor", toolManager->getShopTool()->getShopEditor());
    engine.rootContext()->setContextProperty("item_editor", toolManager->getShopTool()->getShopEditor()->getItemEditor());

    // Characters
    engine.rootContext()->setContextProperty("character_tool", toolManager->getCharacterTool());
    engine.rootContext()->setContextProperty("character_image_viewer", toolManager->getCharacterTool()->getImageViewer());
    engine.rootContext()->setContextProperty("character_dsa5_viewer", toolManager->getCharacterTool()->getDSA5Viewer());

    // Other Tools
    engine.rootContext()->setContextProperty("map_tool", toolManager->getMapTool());
    engine.rootContext()->setContextProperty("dice_tool", toolManager->getDiceTool());
    engine.rootContext()->setContextProperty("name_generator", toolManager->getNameGenerator());
    engine.rootContext()->setContextProperty("notes_tool", toolManager->getNotesTool());
    engine.rootContext()->setContextProperty("converter_tool", toolManager->getConverterTool());
    engine.rootContext()->setContextProperty("settings_tool", toolManager->getSettingsTool());

    // Misc
    engine.rootContext()->setContextProperty("update_manager", new UpdateManager);
    engine.rootContext()->setContextProperty("platform", new PlatformDetails);
    engine.rootContext()->setContextProperty("color_scheme", new ColorScheme);
    engine.rootContext()->setContextProperty("addon_manager", new AddonManager);

    // Services
    engine.rootContext()->setContextProperty("spotify_service", Spotify::getInstance());

    engine.load(source);

    if (engine.rootObjects().isEmpty()) return -1;

    return QGuiApplication::exec();
}
