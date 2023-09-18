#include "models/treeitem.h"
#include <gtest/gtest.h>

class CheckItem : public TreeItem
{
    Q_OBJECT
public:
    explicit CheckItem(const QString &name, int depth, bool canToggle, QObject *parent)
        : TreeItem(name, depth, canToggle, parent)
    {
    }

    [[nodiscard]] auto isCheckable() const -> bool override
    {
        return true;
    }
};

TEST(TreeItemTest, CanChangeProperties)
{
    TreeItem item(QStringLiteral("item"), 0, true);
    EXPECT_EQ(item.name(), QStringLiteral("item"));
    EXPECT_EQ(item.depth(), 0);
    EXPECT_FALSE(item.isOpen());

    item.rename(QStringLiteral("root"));
    EXPECT_EQ(item.name(), QStringLiteral("root"));
    EXPECT_EQ(item.path(), QStringLiteral("root"));

    item.isOpen(false);
    item.toggle();
    EXPECT_TRUE(item.isOpen());
}

TEST(TreeItemTest, CanAddAndModifyChildren)
{
    TreeItem root(QStringLiteral("root"), 0, true);
    EXPECT_TRUE(root.childItems().isEmpty());

    TreeItem child0(QStringLiteral("child0"), 1, true, &root);
    EXPECT_EQ(child0.path(), QStringLiteral("root/child0"));

    root.isOpen(false);
    EXPECT_EQ(root.childItems().length(), 0);

    root.isOpen(true);
    EXPECT_EQ(root.childItems().length(), 1);

    TreeItem child1(QStringLiteral("child1"), 1, true, &root);
    EXPECT_EQ(child1.path(), QStringLiteral("root/child1"));
    EXPECT_EQ(root.childItems().length(), 2);
    EXPECT_EQ(root.childItems().at(1)->name(), QStringLiteral("child1"));

    child1.priority(0);
    EXPECT_EQ(root.childItems().at(0)->name(), QStringLiteral("child1"));
}

TEST(TreeItemTest, CanCheckItemAndChildren)
{
    CheckItem root(QStringLiteral("root"), 0, true, nullptr);
    EXPECT_TRUE(root.isCheckable());

    root.setIsChecked(TreeItem::CheckedState::Checked);
    EXPECT_EQ(root.isChecked(), TreeItem::CheckedState::Checked);

    root.setIsChecked(TreeItem::CheckedState::Unchecked);
    EXPECT_EQ(root.isChecked(), TreeItem::CheckedState::Unchecked);

    CheckItem child0(QStringLiteral("child0"), 1, true, &root);
    child0.setIsChecked(TreeItem::CheckedState::Unchecked);
    CheckItem child1(QStringLiteral("child1"), 1, true, &root);
    child1.setIsChecked(TreeItem::CheckedState::Unchecked);
    root.isOpen(true);
    EXPECT_EQ(root.childItems().length(), 2);

    // one child checked sets root to partially checked
    child0.setIsChecked(TreeItem::CheckedState::Checked);
    EXPECT_EQ(child0.isChecked(), TreeItem::CheckedState::Checked);
    EXPECT_EQ(root.isChecked(), TreeItem::CheckedState::PartiallyChecked);

    // children unchecked sets root to unchecked
    child0.setIsChecked(TreeItem::CheckedState::Unchecked);
    EXPECT_EQ(root.isChecked(), TreeItem::CheckedState::Unchecked);

    // children checked sets root to checked
    child0.setIsChecked(TreeItem::CheckedState::Checked);
    child1.setIsChecked(TreeItem::CheckedState::Checked);
    EXPECT_EQ(root.isChecked(), TreeItem::CheckedState::Checked);

    // checking root sets children to checked
    child0.setIsChecked(TreeItem::CheckedState::Unchecked);
    child1.setIsChecked(TreeItem::CheckedState::Unchecked);
    root.setIsChecked(TreeItem::CheckedState::Checked);
    EXPECT_EQ(child0.isChecked(), TreeItem::CheckedState::Checked);
    EXPECT_EQ(child1.isChecked(), TreeItem::CheckedState::Checked);
}

#include "testtreeitem.moc"
