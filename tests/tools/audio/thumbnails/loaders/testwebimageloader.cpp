#include <QtTest>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "src/tools/audio/thumbnails/loaders/webimageloader.h"
#include "tests/testhelper/abstracttest.h"

class TestWebImageLoader : public AbstractTest
{
    Q_OBJECT

private:
    QNetworkAccessManager m_networkManager;

private slots:
    void initTestCase();

    void loadImageAsync_data();
    void loadImageAsync();
};

void TestWebImageLoader::initTestCase()
{
    m_networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

void TestWebImageLoader::loadImageAsync_data()
{
    QTest::addColumn<QString>("url");

    QTest::newRow("png") << "https://sipi.usc.edu/database/preview/textures/1.1.01.png";
}

void TestWebImageLoader::loadImageAsync()
{
    QFETCH(QString, url);

    const auto future = WebImageLoader::loadImageAsync(url, &m_networkManager);
    testFuture(future, "loadImageAsync", [future] {
        const auto pixmap = future.result();
        QVERIFY(!pixmap.isNull());
    });
}

QTEST_MAIN(TestWebImageLoader)
#include "testwebimageloader.moc"
