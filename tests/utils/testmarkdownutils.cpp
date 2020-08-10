#include <QtTest>
#include <QObject>
#include "../../src/utils/markdownutils.h"

class TestMarkdownUtils : public QObject
{
    Q_OBJECT
public:
    TestMarkdownUtils() {}

private slots:
    void markdownToHtml();
};

void TestMarkdownUtils::markdownToHtml()
{
    auto markdown = QStringLiteral("# Test 1\n\nThis is a test.");

    QVERIFY(MarkdownUtils::markdownToHtml(markdown).contains("<h1>Test 1</h1>"));
}

QTEST_APPLESS_MAIN(TestMarkdownUtils)
#include "testmarkdownutils.moc"
