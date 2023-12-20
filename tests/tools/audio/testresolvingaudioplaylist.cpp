#include "settings/settingsmanager.h"
#include "src/services/youtube/youtube.h"
#include "src/tools/audio/playlist/resolvingaudioplaylist.h"
#include "testhelper/abstractmocknetworkmanager.h"
#include "testhelper/abstracttest.h"
#include "testhelper/mocknetworkreply.h"
#include "utils/fileutils.h"
#include <QDesktopServices>
#include <gtest/gtest.h>

#ifndef QT_GUI_LIB
#define QT_GUI_LIB
#endif

using namespace Qt::Literals::StringLiterals;
using namespace Common::Settings;

class PlaylistMockNetworkManager : public AbstractMockNetworkManager
{
    Q_OBJECT
public:
    explicit PlaylistMockNetworkManager(const QString &host, QObject *parent = nullptr)
        : AbstractMockNetworkManager({host}, parent)
    {
    }

protected:
    auto sendMockReply(Operation, const QNetworkRequest &req, const QByteArray &) -> QNetworkReply * override
    {
        QByteArray data;
        QFile file(u":"_s + req.url().path());
        if (file.open(QIODevice::ReadOnly))
        {
            data = file.readAll();
            file.close();
            emit replySent();
            return MockNetworkReply::successGeneric(data, this);
        }

        emit replySent();
        return MockNetworkReply::notFound(this);
    }
};

class ResolvingAudioPlaylistTest : public AbstractTest
{
public:
    ResolvingAudioPlaylistTest()
    {
        networkManager = std::make_unique<PlaylistMockNetworkManager>(u"fileserver.mock"_s);
        QDesktopServices::setUrlHandler(u"http"_s, networkManager.get(), "simulateBrowser");
        QDesktopServices::setUrlHandler(u"https"_s, networkManager.get(), "simulateBrowser");

        Services::YouTube::instance()->setNetworkManager(networkManager.get());

        m_playlist = std::make_unique<ResolvingAudioPlaylist>(u"testing"_s, networkManager.get());

        auto testingDir = SettingsManager::getPath(u"testing"_s);
        backupDir = backupUserFolder(testingDir);

        copyResourceToFile(u":/resources/audioplaylist/test.m3u"_s, FileUtils::fileInDir(u"test.m3u"_s, testingDir));
        copyResourceToFile(u":/resources/audioplaylist/test.pls"_s, FileUtils::fileInDir(u"test.pls"_s, testingDir));
    }

    ~ResolvingAudioPlaylistTest() override
    {
        restoreUserFolder(backupDir, SettingsManager::getPath(u"testing"_s));
    }

protected:
    std::unique_ptr<ResolvingAudioPlaylist> m_playlist = nullptr;

private:
    QString backupDir;
};

TEST_F(ResolvingAudioPlaylistTest, CanResolveEmptyPlaylist)
{
    auto future = m_playlist->resolve();
    testFuture(future, "resolve()", [this, future]() {
        EXPECT_FALSE(future.isCanceled());
        EXPECT_TRUE(m_playlist->isEmpty());
    });
}

TEST_F(ResolvingAudioPlaylistTest, CanResolveLocalPlaylists)
{
    auto *m3u = new AudioFile(u"/test.m3u"_s, AudioFile::Source::File, u""_s, nullptr);
    auto *pls = new AudioFile(u"/test.pls"_s, AudioFile::Source::File, u""_s, nullptr);
    m_playlist->setFiles({m3u, pls});

    auto future = m_playlist->resolve();
    testFuture(future, "resolve()", [this, future]() {
        EXPECT_FALSE(future.isCanceled());
        EXPECT_FALSE(m_playlist->isEmpty());
        EXPECT_EQ(m_playlist->length(), 6);
    });
}

TEST_F(ResolvingAudioPlaylistTest, CanResolveWebPlaylists)
{
    auto *m3u = new AudioFile(u"http://fileserver.mock/resources/audioplaylist/test.m3u"_s, AudioFile::Source::Web,
                              u""_s, nullptr);
    auto *pls = new AudioFile(u"http://fileserver.mock/resources/audioplaylist/test.pls"_s, AudioFile::Source::Web,
                              u""_s, nullptr);
    m_playlist->setFiles({m3u, pls});

    auto future = m_playlist->resolve();
    testFuture(future, "resolve()", [this, future]() {
        EXPECT_FALSE(future.isCanceled());
        EXPECT_FALSE(m_playlist->isEmpty());
        EXPECT_EQ(m_playlist->length(), 6);
    });
}

TEST_F(ResolvingAudioPlaylistTest, CanResolveYouTubePlaylist)
{
    auto *yt = new AudioFile(u"https://www.youtube.com/playlist?list=PL53mjgVKFq7yu0LdAvpp42ZGLzRCkFKuz"_s,
                             AudioFile::Source::Youtube, u""_s, nullptr);
    m_playlist->setFiles({yt});

    auto future = m_playlist->resolve();
    testFuture(future, "resolve()", [this, future]() {
        EXPECT_FALSE(future.isCanceled());
        EXPECT_FALSE(m_playlist->isEmpty());
        EXPECT_GT(m_playlist->length(), 1);
    });
}

TEST_F(ResolvingAudioPlaylistTest, CanResolveMixedPlaylist)
{
    auto *m3u = new AudioFile(u"/test.m3u"_s, AudioFile::Source::File, u""_s, nullptr);
    auto *pls = new AudioFile(u"/test.pls"_s, AudioFile::Source::File, u""_s, nullptr);
    auto *yt = new AudioFile(u"https://www.youtube.com/playlist?list=PL53mjgVKFq7yu0LdAvpp42ZGLzRCkFKuz"_s,
                             AudioFile::Source::Youtube, u""_s, nullptr);
    m_playlist->setFiles({m3u, pls, yt});

    auto future = m_playlist->resolve();
    testFuture(future, "resolve()", [this, future]() {
        EXPECT_FALSE(future.isCanceled());
        EXPECT_FALSE(m_playlist->isEmpty());
        EXPECT_GT(m_playlist->length(), 7);
    });
}

#include "testresolvingaudioplaylist.moc"
