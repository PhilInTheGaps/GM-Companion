#include "models/treeitem.h"
#include "qtestcase.h"
#include <QObject>
#include <QtTest>

class TestTreeItem : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

private slots:
    void canChangeProperties();
    void canAddAndModifyChildren();
    void canCheckItemAndChildren();
};

class CheckItem : public TreeItem
{
    Q_OBJECT
public:
    explicit CheckItem(QString name, int depth, bool canToggle, QObject *parent)
        : TreeItem(std::move(name), depth, canToggle, parent)
    {
    }

    [[nodiscard]] auto isCheckable() const -> bool override
    {
        return true;
    }
};

void TestTreeItem::canChangeProperties()
{
    TreeItem item(QStringLiteral("item"), 0, true);
    QCOMPARE(item.name(), QStringLiteral("item"));
    QCOMPARE(item.depth(), 0);
    QVERIFY(!item.isOpen());

    item.rename(QStringLiteral("root"));
    QCOMPARE(item.name(), QStringLiteral("root"));
    QCOMPARE(item.path(), QStringLiteral("root"));

    item.isOpen(false);
    item.toggle();
    QVERIFY(item.isOpen());
}

void TestTreeItem::canAddAndModifyChildren()
{
    TreeItem root(QStringLiteral("root"), 0, true);
    QVERIFY(root.childItems().isEmpty());

    TreeItem child0(QStringLiteral("child0"), 1, true, &root);
    QCOMPARE(child0.path(), QStringLiteral("root/child0"));

    root.isOpen(false);
    QCOMPARE(root.childItems().length(), 0);

    root.isOpen(true);
    QCOMPARE(root.childItems().length(), 1);

    TreeItem child1(QStringLiteral("child1"), 1, true, &root);
    QCOMPARE(child1.path(), QStringLiteral("root/child1"));
    QCOMPARE(root.childItems().length(), 2);
    QCOMPARE(qobject_cast<TreeItem *>(root.childItems().at(1))->name(), QStringLiteral("child1"));

    child1.priority(0);
    QCOMPARE(qobject_cast<TreeItem *>(root.childItems().at(0))->name(), QStringLiteral("child1"));
}

void TestTreeItem::canCheckItemAndChildren()
{
    CheckItem root(QStringLiteral("root"), 0, true, this);
    QVERIFY(root.isCheckable());

    root.setIsChecked(TreeItem::CheckedState::Checked);
    QCOMPARE(root.isChecked(), TreeItem::CheckedState::Checked);

    root.setIsChecked(TreeItem::CheckedState::Unchecked);
    QCOMPARE(root.isChecked(), TreeItem::CheckedState::Unchecked);

    CheckItem child0(QStringLiteral("child0"), 1, true, &root);
    child0.setIsChecked(TreeItem::CheckedState::Unchecked);
    CheckItem child1(QStringLiteral("child1"), 1, true, &root);
    child1.setIsChecked(TreeItem::CheckedState::Unchecked);
    root.isOpen(true);
    QCOMPARE(root.childItems().length(), 2);

    // one child checked sets root to partially checked
    child0.setIsChecked(TreeItem::CheckedState::Checked);
    QCOMPARE(child0.isChecked(), TreeItem::CheckedState::Checked);
    QCOMPARE(root.isChecked(), TreeItem::CheckedState::PartiallyChecked);

    // children unchecked sets root to unchecked
    child0.setIsChecked(TreeItem::CheckedState::Unchecked);
    QCOMPARE(root.isChecked(), TreeItem::CheckedState::Unchecked);

    // children checked sets root to checked
    child0.setIsChecked(TreeItem::CheckedState::Checked);
    child1.setIsChecked(TreeItem::CheckedState::Checked);
    QCOMPARE(root.isChecked(), TreeItem::CheckedState::Checked);

    // checking root sets children to checked
    child0.setIsChecked(TreeItem::CheckedState::Unchecked);
    child1.setIsChecked(TreeItem::CheckedState::Unchecked);
    root.setIsChecked(TreeItem::CheckedState::Checked);
    QCOMPARE(child0.isChecked(), TreeItem::CheckedState::Checked);
    QCOMPARE(child1.isChecked(), TreeItem::CheckedState::Checked);
}

QTEST_APPLESS_MAIN(TestTreeItem)
#include "testtreeitem.moc"
