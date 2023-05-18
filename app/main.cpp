#include <QFontDatabase>
#include <QFuture>
#include <QGuiApplication>
#include <QIcon>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSGRendererInterface>
#include <QScopeGuard>
#include <QTranslator>
#include <QtQuickControls2/QQuickStyle>

#include <sentry.h>

#include "addons/addonmanager.h"
#include "filesystem/file.h"
#include "filesystem/fileaccessswitcher.h"
#include "filesystem/filedialog/filedialog.h"
#include "logger.h"
#include "messages/messagemanager.h"
#include "services/google/googledrive.h"
#include "services/nextcloud/nextcloud.h"
#include "services/spotify/spotify.h"
#include "settings/quicksettingsmanager.h"
#include "tools.h"
#include "updates/updatemanager.h"

Q_LOGGING_CATEGORY(gmMain, "gm.main")

Q_DECLARE_METATYPE(QFuture<void>)

/// Register meta types for signals and slots
void registerMetaTypes()
{
    qmlRegisterType<Files::FileDialog>("lol.rophil.gmcompanion.filedialog", 1, 0, "FileDialogBackend");
    qmlRegisterSingletonType<Files::FileAccessSwitcher>("lol.rophil.gmcompanion.fileaccessswitcher", 1, 0,
                                                        "FileAccessSwitcher",
                                                        Files::FileAccessSwitcher::QmlSingletonProvider);

    qRegisterMetaType<AudioElement *>();
    qRegisterMetaType<QList<AudioProject *>>();
    qRegisterMetaType<QList<QNetworkReply::RawHeaderPair>>();
    qRegisterMetaType<QFuture<void>>();
}

/// Set the language and install a translator
void initTranslations()
{
    qCDebug(gmMain()) << "Initializing translations ...";
    auto *translator = new QTranslator;

    if (translator->load(SettingsManager::getLanguage(), QStringLiteral("gm-companion"), QStringLiteral("_"),
                         QStringLiteral(":/translations")))
    {
        QGuiApplication::installTranslator(translator);

        qCDebug(gmMain()) << "Loaded translation" << SettingsManager::getLanguage();
    }
    else
    {
        qCWarning(gmMain) << "Could not load translation";
    }
}

/// Install fonts for FontAwesome.pri
void initResources()
{
    Q_INIT_RESOURCE(fontawesome);
    Q_INIT_RESOURCE(libresources);
    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/fa-solid.ttf"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/fa-regular.ttf"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/fonts/fa-brands.ttf"));
}

auto main(int argc, char *argv[]) -> int
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(QStringLiteral("GM-Companion"));
    QGuiApplication::setOrganizationName(QStringLiteral("GM-Companion"));
    QGuiApplication::setOrganizationDomain(QStringLiteral("gm-companion.github.io"));
    QGuiApplication::setWindowIcon(QIcon(":/icons/icon.png"));

#if defined(Q_OS_WIN)
    app.setFont(QFont("Segoe UI"));
#endif // if defined(Q_OS_WIN)

    const Logger logger;

    qCDebug(gmMain()).noquote() << "Starting GM-Companion" << CURRENT_VERSION << "...";

    registerMetaTypes();
    initTranslations();
    initResources();

    // Sentry.io crash reporting
    // Crash reports and session tracking are opt-in settings
    if (SettingsManager::instance()->get(QStringLiteral("crashReports"), false, QStringLiteral("Telemetry")))
    {
        qCDebug(gmMain()) << "Crash reports are enabled!";
        auto *sentryOptions = sentry_options_new();
        sentry_options_set_dsn(sentryOptions,
                               "https://e42ba403690043fa8fdd4216a4c58a08@o1229208.ingest.sentry.io/6375554");
        sentry_options_set_release(sentryOptions,
                                   QStringLiteral("gm-companion@%1").arg(CURRENT_VERSION).toUtf8().data());

        auto isSessionTrackingEnabled =
            SettingsManager::instance()->get(QStringLiteral("sessionTracking"), false, QStringLiteral("Telemetry"));
        sentry_options_set_auto_session_tracking(sentryOptions, isSessionTrackingEnabled ? 1 : 0);
        if (isSessionTrackingEnabled) qCDebug(gmMain()) << "Session tracking is enabled!";

        sentry_set_tag("qt", qVersion());

        sentry_init(sentryOptions);
    }

    auto sentryClose = qScopeGuard([] { sentry_close(); });

    // Make classes available for QML
    QUrl const source(QStringLiteral("qrc:/main.qml"));

    QQuickStyle::setStyle(QStringLiteral(":/style"));

    QQmlApplicationEngine engine;
    engine.addImportPath(QStringLiteral("qrc:/"));

    // Misc
    QNetworkAccessManager networkManager(nullptr);
    networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    NextCloud nc(networkManager, nullptr);
    GoogleDrive gd(networkManager, nullptr);
    Files::File::init(&nc, &gd);

    engine.rootContext()->setContextProperty(QStringLiteral("settings_manager"), new QuickSettingsManager);
    engine.rootContext()->setContextProperty(QStringLiteral("update_manager"), new UpdateManager);
    engine.rootContext()->setContextProperty(QStringLiteral("addon_manager"), AddonManager::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("addon_repository_manager"),
                                             &(AddonManager::instance()->repositoryManager()));
    engine.rootContext()->setContextProperty(QStringLiteral("message_manager"), MessageManager::instance());
    engine.addImageProvider(QStringLiteral("audioElementIcons"), new AudioThumbnailProvider);

    // Services
    engine.rootContext()->setContextProperty(QStringLiteral("spotify_service"), Spotify::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("googledrive_service"), &gd);
    engine.rootContext()->setContextProperty(QStringLiteral("nextcloud_service"), &nc);

    // Load tools
    const AudioTool audioTool(&engine);
    const MapTool mapTool(&engine);
    const DiceTool diceTool(&engine);
    const CombatTracker combatTracker(&engine);
    const ShopTool shopTool(&engine);
    const CharacterTool characterTool(&engine);
    const NameGenerator nameGenerator(&engine);
    const NotesTool notesTool(&engine);
    const ConverterTool converterTool(&engine);

    engine.load(source);

    if (engine.rootObjects().isEmpty()) return -1;

    return QGuiApplication::exec();
}
