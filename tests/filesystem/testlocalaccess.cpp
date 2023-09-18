#include "abstractaccesstest.h"
#include "src/filesystem/fileaccesslocal.h"
#include <gtest/gtest.h>

using namespace Files;

class LocalAccessTest : public AbstractAccessTest
{
public:
    LocalAccessTest()
    {
        fileAccess = std::make_unique<FileAccessLocal>(nullptr);
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
