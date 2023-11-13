#include "settings/settingsmanager.h"
#include "staticabstracttest.h"
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;
using namespace Common::Settings;

TEST(SettingsManagerTest, CanReadWritePassword)
{
    ASSERT_TRUE(SettingsManager::instance());

    const auto username = u"TestUser"_s;
    const auto password = u"TestPassword"_s;
    const auto service = u"TestService"_s;

    auto save = SettingsManager::setPassword(username, password, service);
    EXPECT_TRUE(save);

    auto read = SettingsManager::getPassword(username, service);
    EXPECT_EQ(read.toStdString(), password.toStdString());

    auto save2 = SettingsManager::setPassword(username, u""_s, service);
    EXPECT_TRUE(save2);

    auto read2 = SettingsManager::getPassword(username, service);
    EXPECT_TRUE(read2.isEmpty());
}
