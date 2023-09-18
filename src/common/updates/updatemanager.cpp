#include "updatemanager.h"
#include "utils/stringutils.h"
#include "version.h"
#include <QLoggingCategory>
#include <QXmlStreamReader>

Q_LOGGING_CATEGORY(gmUpdateManager, "gm.updates.manager")

constexpr ConstQString DEFAULT_FEED_URL = "https://github.com/PhilInTheGaps/GM-Companion/releases.atom";

using namespace Qt::Literals::StringLiterals;

UpdateManager::UpdateManager(QObject *parent) : QObject(parent)
{
    qCDebug(gmUpdateManager()) << "Initializing UpdateManager ...";

    // GitHub Release feed
    m_feedURL = DEFAULT_FEED_URL;

    networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    checkSslInstallation();
}

/// Check if openSSL is installed, required for network access to work
void UpdateManager::checkSslInstallation()
{
    qCDebug(gmUpdateManager()) << "Checking SSL installation...";

    if (!QSslSocket::supportsSsl())
        qCWarning(gmUpdateManager()) << "Please install OpenSSL" << QSslSocket::sslLibraryBuildVersionString();
    else
        qCDebug(gmUpdateManager()) << "SSL version" << QSslSocket::sslLibraryVersionString() << "is installed.";
}

/**
 * @brief Check the release feed if updates are available
 */
void UpdateManager::checkForUpdates()
{
    qCDebug(gmUpdateManager) << "Checking for updates ...";
    qCDebug(gmUpdateManager) << "Current version:" << CURRENT_VERSION;
    qCDebug(gmUpdateManager) << "Releases feed URL:" << m_feedURL;

    auto version = fetchNewestVersion();
    version.then(this, [this](const QString &version) {
        m_newestVersion = version;
        qCDebug(gmUpdateManager()) << "Newest available version:" << version;

        if (compareVersions(CURRENT_VERSION, m_newestVersion))
        {
            qCDebug(gmUpdateManager()) << "Your version" << CURRENT_VERSION << "is the newest one.";
            emit noUpdateAvailable();
        }
        else
        {
            qCDebug(gmUpdateManager()) << "Found a newer version:" << m_newestVersion;
            emit updateAvailable();
        }
    });
}

auto UpdateManager::getNewestVersion() const -> QString
{
    return m_newestVersion;
}

auto UpdateManager::getCurrentVersion() -> QString
{
    return CURRENT_VERSION;
}

/**
 * @brief Get if version 1 is newer than version 2
 * @param v1 Version 1
 * @param v2 Version 2
 * @return True if v1 is newer than v2
 */
auto UpdateManager::compareVersions(const QString &v1, const QString &v2) -> bool
{
    return Version::isGreater(Version(v1), Version(v2));
}

auto UpdateManager::findVersionsFromXML(const QByteArray &xml) -> QStringList
{
    QXmlStreamReader reader(xml);
    QStringList versions;

    if (!reader.readNextStartElement() || reader.name() != "feed"_L1) return {};

    while (reader.readNextStartElement())
    {
        if (reader.name() != "entry"_L1)
        {
            reader.skipCurrentElement();
            continue;
        }

        QString id;

        // Read every xml entry and check if it is useful information
        while (reader.readNextStartElement())
        {
            if (reader.name() == "id"_L1) id = reader.readElementText();

            else
                reader.skipCurrentElement();
        }

        // Version is converted from git tag, so we have to remove a bunch of junk
        versions << id.replace(u"tag:github.com,2008:Repository/78660365/"_s, ""_L1);
    }

    return versions;
}

auto UpdateManager::findNewestVersion(const QStringList &versions) -> QString
{
    QString newest;

    for (const auto &version : versions)
    {
        if (compareVersions(version, newest)) newest = version;
    }

    return newest;
}

auto UpdateManager::fetchNewestVersion() -> QFuture<QString>
{
    // Get the release feed to check for a new version
    auto *reply = networkManager.get(QNetworkRequest(QUrl(m_feedURL)));

    return QtFuture::connect(reply, &QNetworkReply::finished).then(this, [reply]() {
        auto versions = findVersionsFromXML(reply->readAll());
        reply->deleteLater();

        qCDebug(gmUpdateManager()) << "Found the following versions:" << versions;

        return findNewestVersion(versions);
    });
}
