#include "src/tools/audio/editor/addonelementmanager.h"
#include "tests/testhelper/abstracttest.h"

class AudioAddonsTest : public AbstractTest
{
public:
    explicit AudioAddonsTest()
    {
        m_testAddons = enableTestAddons();
    }

    ~AudioAddonsTest()
    {
        disableTestAddons(m_testAddons);
    }

private:
    QList<Addon *> m_testAddons;
};

TEST_F(AudioAddonsTest, CanFindAddonsWithAudioContent)
{
    AddonElementManager manager;
    EXPECT_EQ(manager.addons().length(), 0);

    manager.loadData();

    if (manager.isLoading())
    {
        QSignalSpy spy(&manager, &AddonElementManager::isLoadingChanged);
        EXPECT_TRUE(spy.wait());
    }

    EXPECT_EQ(manager.addons().length(), 2);

    manager.currentIndex(0);
    const auto projects = manager.projects();
    EXPECT_EQ(projects.length(), 1);
}
