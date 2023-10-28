#include "settings/settingsmanager.h"
#include "staticabstracttest.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

TEST(SettingsManagerTest, CanReadWritePassword)
{
    ASSERT_TRUE(SettingsManager::instance());

    const auto username = u"TestUser"_s;
    const auto password = u"TestPassword"_s;
    const auto service = u"TestService"_s;

    auto save = SettingsManager::setPassword(username, password, service);
    StaticAbstractTest::testFutureNoAuth(save, "SettingsManager::getPassword",
                                         [password, save]() { EXPECT_TRUE(save.result()); });

    auto read = SettingsManager::getPassword(username, service);
    StaticAbstractTest::testFutureNoAuth(read, "SettingsManager::getPassword", [password, read]() {
        EXPECT_EQ(read.result().toStdString(), password.toStdString());
    });

    auto save2 = SettingsManager::setPassword(username, u""_s, service);
    StaticAbstractTest::testFutureNoAuth(save2, "SettingsManager::getPassword",
                                         [password, save2]() { EXPECT_TRUE(save2.result()); });

    auto read2 = SettingsManager::getPassword(username, service);
    StaticAbstractTest::testFutureNoAuth(read2, "SettingsManager::getPassword",
                                         [read2]() { EXPECT_TRUE(read2.result().isEmpty()); });
}
