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

using namespace Qt::Literals::StringLiterals;

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

    if (translator->load(SettingsManager::getLanguage(), u"gm-companion"_s, u"_"_s, u":/i18n"_s))
    {
        QGuiApplication::installTranslator(translator);

        qCDebug(gmMain()) << "Loaded translation" << SettingsManager::getLanguage();
    }
    else
    {
        qCWarning(gmMain) << "Could not load translation";
    }
}

void initResources()
{
    Q_INIT_RESOURCE(libresources);
    Q_INIT_RESOURCE(services_resources);
}

auto main(int argc, char *argv[]) -> int
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(u"GM-Companion"_s);
    QGuiApplication::setOrganizationName(u"GM-Companion"_s);
    QGuiApplication::setOrganizationDomain(u"gm-companion.github.io"_s);
    QGuiApplication::setWindowIcon(QIcon(u":/resources/icons/icon.png"_s));

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
    if (SettingsManager::instance()->get(u"crashReports"_s, false, u"Telemetry"_s))
    {
        qCDebug(gmMain()) << "Crash reports are enabled!";
        auto *sentryOptions = sentry_options_new();
        sentry_options_set_dsn(sentryOptions,
                               "https://e42ba403690043fa8fdd4216a4c58a08@o1229208.ingest.sentry.io/6375554");
        sentry_options_set_release(sentryOptions, u"gm-companion@%1"_s.arg(CURRENT_VERSION).toUtf8().data());

        auto isSessionTrackingEnabled = SettingsManager::instance()->get(u"sessionTracking"_s, false, u"Telemetry"_s);
        sentry_options_set_auto_session_tracking(sentryOptions, isSessionTrackingEnabled ? 1 : 0);
        if (isSessionTrackingEnabled) qCDebug(gmMain()) << "Session tracking is enabled!";

        sentry_set_tag("qt", qVersion());

        sentry_init(sentryOptions);
    }

    auto sentryClose = qScopeGuard([] { sentry_close(); });

    QQuickStyle::setStyle(u"Style"_s);
    QQmlApplicationEngine engine;

    // Misc
    QNetworkAccessManager networkManager(nullptr);
    networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    NextCloud nc(networkManager, nullptr);
    GoogleDrive gd(networkManager, nullptr);
    Files::File::init(&nc, &gd);

    engine.rootContext()->setContextProperty(u"settings_manager"_s, new QuickSettingsManager);
    engine.rootContext()->setContextProperty(u"update_manager"_s, new UpdateManager);
    engine.rootContext()->setContextProperty(u"addon_manager"_s, AddonManager::instance());
    engine.rootContext()->setContextProperty(u"addon_repository_manager"_s,
                                             &(AddonManager::instance()->repositoryManager()));
    engine.rootContext()->setContextProperty(u"message_manager"_s, MessageManager::instance());
    engine.addImageProvider(u"audioElementIcons"_s, new AudioThumbnailProvider);

    // Services
    engine.rootContext()->setContextProperty(u"spotify_service"_s, Spotify::instance());
    engine.rootContext()->setContextProperty(u"googledrive_service"_s, &gd);
    engine.rootContext()->setContextProperty(u"nextcloud_service"_s, &nc);

    // Load tools
    const AudioTool audioTool(&engine, networkManager);
    const MapTool mapTool(&engine);
    const DiceTool diceTool(&engine);
    const CombatTracker combatTracker(&engine);
    const ShopTool shopTool(&engine);
    const CharacterTool characterTool(&engine);
    const NameGenerator nameGenerator(&engine);
    const NotesTool notesTool(&engine);
    const ConverterTool converterTool(&engine);

    engine.loadFromModule("ui"_L1, "Main"_L1);

    if (engine.rootObjects().isEmpty()) return -1;

    return QGuiApplication::exec();
}
