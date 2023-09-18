#include "updates/updatemanager.h"
#include <QFutureWatcher>
#include <QObject>
#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

static constexpr int WAIT_TIME_IN_MS = 10000;

struct VersionComparison
{
    const char *description;
    QString version1;
    QString version2;
    bool isNewer;
};

TEST(UpdateManagerTest, CompareVersions)
{
    std::vector<VersionComparison> comparisons;

    comparisons.push_back({"newer major", "2.0.1", "1.2.0", true});
    comparisons.push_back({"newer patch", "1.2.1", "1.2.0", true});
    comparisons.push_back({"newer minor", "1.3.0", "1.2.0", true});
    comparisons.push_back({"newer minor (short)", "1.1", "1.0.2", true});
    comparisons.push_back({"newer major (short)", "2", "1.3.4", true});
    comparisons.push_back({"newer major (long)", "1.1.0", "0.3.1.1", true});

    comparisons.push_back({"older patch", "1.2.1", "1.2.2", false});
    comparisons.push_back({"older minor", "1.1.1", "1.2.1", false});
    comparisons.push_back({"older patch 2", "1.0.1", "1.0.2", false});
    comparisons.push_back({"older major (short)", "0", "1", false});
    comparisons.push_back({"older empty", "", "1.0.0", false});
    comparisons.push_back({"older patch (short)", "1.0", "1.0.1", false});
    comparisons.push_back({"older major (long)", "0.3.1.1", "1.1.0", false});

    comparisons.push_back({"newer dev rc/beta", "1.0.0-rc1", "1.0.0-beta3", true});
    comparisons.push_back({"newer dev major", "1.0.0-alpha1", "0.9.3", true});
    comparisons.push_back({"newer dev alpha2", "1.0.0-alpha2", "1.0.0-alpha1", true});
    comparisons.push_back({"newer dev beta/alpha", "1.0.0-beta1", "1.0.0-alpha1", true});
    comparisons.push_back({"newer dev beta/alpha4", "1.0.0-beta1", "1.0.0-alpha4", true});
    comparisons.push_back({"newer dev releas/alpha", "1.0.0", "1.0.0-alpha4", true});
    comparisons.push_back({"newer dev minor+alpha", "1.2-alpha1", "1.1.3", true});

    comparisons.push_back({"older dev alpha/none", "1.0.0-alpha1", "1.0.0", false});
    comparisons.push_back({"older dev alpha1/2", "1.0.0-alpha1", "1.0.0-alpha2", false});
    comparisons.push_back({"older dev alpha/beta", "1.0.0-alpha1", "1.0.0-beta1", false});

    for (const auto &comparison : comparisons)
    {
        EXPECT_EQ(UpdateManager::compareVersions(comparison.version1, comparison.version2), comparison.isNewer)
            << comparison.description;
        EXPECT_NE(UpdateManager::compareVersions(comparison.version2, comparison.version1), comparison.isNewer)
            << comparison.description;
    }
}

class UpdateManagerTester : public UpdateManager
{
public:
    explicit UpdateManagerTester() : UpdateManager(nullptr)
    {
    }

    void canFindNewestVersion()
    {
        const QStringList versions = {"1.1.0",   "1.0.2",   "1.0.1.0", "1.0.0.0", "0.3.3.0",
                                      "0.3.2.0", "0.3.1.1", "0.3.1.0", "0.3.00",  "0.3.0.1"};

        EXPECT_EQ(findNewestVersion(versions), "1.1.0"_L1);
    }

    void canFetchNewestVersion()
    {
        QFutureWatcher<QString> watcher;
        QSignalSpy spy(&watcher, &QFutureWatcher<QString>::finished);

        auto future = fetchNewestVersion();
        watcher.setFuture(future);
        EXPECT_FALSE(future.isCanceled()) << "The QFuture object returned by fetchNewestVersion() has been canceled.";

        auto waitTime = WAIT_TIME_IN_MS;
        auto isReady = spy.wait(waitTime);

        EXPECT_TRUE(isReady) << "fetchNewestVersion() took longer than"
                             << "ms to respond.";
        EXPECT_FALSE(future.result().isEmpty());
    }
};

TEST(UpdateManagerTest, CanFindNewestVersion)
{
    UpdateManagerTester tester;
    tester.canFindNewestVersion();
}

TEST(UpdateManagerTest, CanFetchNewestVersion)
{
    UpdateManagerTester tester;
    tester.canFetchNewestVersion();
}
