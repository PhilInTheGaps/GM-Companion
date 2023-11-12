#include "filesystem/file.h"
#include "logger.h"
#include "services/nextcloud/nextcloud.h"
#include "settings/quicksettingsmanager.h"
#include "tools/audio/thumbnails/audiothumbnailprovider.h"
#include "updates/version.h"
#include <QFontDatabase>
#include <QFuture>
#include <QGuiApplication>
#include <QIcon>
#include <QLoggingCategory>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSGRendererInterface>
#include <QScopeGuard>
#include <QTranslator>
#include <QtQuickControls2/QQuickStyle>
#include <thirdparty/sentry-native/include/sentry.h>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmMain, "gm.main")

Q_DECLARE_METATYPE(QFuture<void>)

/// Register meta types for signals and slots
void registerMetaTypes()
{
    qRegisterMetaType<QList<QNetworkReply::RawHeaderPair>>();
    qRegisterMetaType<QFuture<void>>();
}

void updateTranslation(QTranslator &translator)
{
    if (!translator.isEmpty()) QCoreApplication::removeTranslator(&translator);

    if (translator.load(SettingsManager::getLanguage(), u"gm-companion"_s, u"_"_s, u":/i18n"_s))
    {
        QGuiApplication::installTranslator(&translator);
        qCDebug(gmMain()) << "Loaded translation" << SettingsManager::getLanguage();
    }
    else
    {
        qCWarning(gmMain) << "Could not load translation";
    }
}

/// Set the language and install a translator
void initTranslations(QTranslator &translator, QQmlEngine &engine)
{
    qCDebug(gmMain()) << "Initializing translations ...";

    updateTranslation(translator);

    QObject::connect(QuickSettingsManager::instance(), &QuickSettingsManager::languageChanged, &translator,
                     [&translator, &engine]() {
                         updateTranslation(translator);
                         engine.retranslate();
                     });
}

void initResources()
{
    Q_INIT_RESOURCE(libresources);
    Q_INIT_RESOURCE(services_resources);
}

void setPalette()
{
    QPalette palette;
    palette.setColor(QPalette::ColorRole::Text, "#f6f7f8"_L1);
    palette.setColor(QPalette::ColorRole::Base, "#2e2e2e"_L1);
    palette.setColor(QPalette::ColorRole::AlternateBase, "#343a43"_L1);
    palette.setColor(QPalette::ColorRole::Window, "#2e2e2e"_L1);
    palette.setColor(QPalette::ColorRole::WindowText, "#f6f7f8"_L1);
    palette.setColor(QPalette::ColorRole::Button, "#8f9aa9"_L1); // 3f4957
    palette.setColor(QPalette::ColorRole::ButtonText, "#f6f7f8"_L1);
    palette.setColor(QPalette::ColorRole::Light, "#ebedef"_L1);
    palette.setColor(QPalette::ColorRole::Midlight, "#d5dade"_L1);
    palette.setColor(QPalette::ColorRole::Mid, "#bfc6cd"_L1);
    palette.setColor(QPalette::ColorRole::Dark, "#1f1f1f"_L1);
    palette.setColor(QPalette::ColorRole::Highlight, "#3f4957"_L1);
    palette.setColor(QPalette::ColorRole::HighlightedText, "#f6f7f8"_L1);
    palette.setColor(QPalette::ColorRole::ToolTipText, "#f6f7f8"_L1);
    palette.setColor(QPalette::ColorRole::ToolTipBase, "#1f1f1f"_L1);

    QGuiApplication::setPalette(palette);
}

auto main(int argc, char *argv[]) -> int
{
    const QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(u"GM-Companion"_s);
    QGuiApplication::setOrganizationName(u"GM-Companion"_s);
    QGuiApplication::setOrganizationDomain(u"gm-companion.github.io"_s);
    QGuiApplication::setWindowIcon(QIcon(u":/resources/icons/icon.png"_s));
    setPalette();

#if defined(Q_OS_WIN)
    QGuiApplication::setFont(QFont("Segoe UI"));
#endif // if defined(Q_OS_WIN)

    const Logger logger;

    qCDebug(gmMain()).noquote() << "Starting GM-Companion" << CURRENT_VERSION << "...";

    registerMetaTypes();
    initResources();

    QQuickStyle::setStyle(u"Style"_s);

    QTranslator translator;
    QQmlApplicationEngine engine;
    initTranslations(translator, engine);

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

    // Misc
    Files::File::init(Services::NextCloud::qmlInstance(&engine));

    engine.addImageProvider(u"audioElementIcons"_s, new AudioThumbnailProvider());
    engine.loadFromModule("ui"_L1, "Main"_L1);

    if (engine.rootObjects().isEmpty()) return -1;

    return QGuiApplication::exec();
}
