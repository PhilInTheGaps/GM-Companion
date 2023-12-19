#include "abstractaccesstest.h"
#include "src/filesystem/file.h"
#include "src/filesystem/fileaccesslocal.h"
#include <gtest/gtest.h>

using namespace Files;

class LocalAccessTest : public AbstractAccessTest
{
public:
    LocalAccessTest()
    {
        fileAccess = std::make_shared<FileAccessLocal>();
        createTestFiles();
    }

protected:
    void createTestDir() override
    {
        // Temp file should have been created automatically
        verifyThatFileExists(getFilePath());
    }

private slots:
    void initTestCase();
};

TEST_F(LocalAccessTest, TestFileAccess)
{
    runAllTests();
}

TEST_F(LocalAccessTest, VerifyHomeDir)
{
    const auto homeDir = File::getHomeDir(fileAccess);
    EXPECT_FALSE(homeDir.isEmpty());
}
