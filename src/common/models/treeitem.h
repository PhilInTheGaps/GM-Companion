#pragma once

#include "baseprojectitem.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>

class TreeItem : public BaseProjectItem
{
    Q_OBJECT

public:
    enum class CheckedState
    {
        Unchecked = 0,
        PartiallyChecked = 1,
        Checked = 2
    };
    Q_ENUM(CheckedState)

    /// Whether children are shown or not.
    AUTO_PROPERTY_VAL2(bool, isOpen, false)

    /// Whether the item can be toggled to display any children.
    AUTO_PROPERTY_VAL2(bool, canToggle, true)

    /// Sorting priority. Smaller means higher priority.
    AUTO_PROPERTY_VAL2(int, priority, INT_MAX)

    Q_PROPERTY(bool isCheckable READ isCheckable CONSTANT)
    Q_PROPERTY(CheckedState isChecked READ isChecked WRITE setIsChecked NOTIFY isCheckedChanged)
    Q_PROPERTY(QList<QObject *> childItems READ childItems NOTIFY childItemsChanged)
    Q_PROPERTY(QStringList creatables READ creatables NOTIFY creatablesChanged)

    READ_PROPERTY(int, depth)

public:
    explicit TreeItem(const QString& name, int depth, bool canToggle = true, QObject *parent = nullptr);

    /// Whether a checkbox should be displayed.
    [[nodiscard]] virtual auto isCheckable() const -> bool;
    [[nodiscard]] auto isChecked() const -> CheckedState;
    void setIsChecked(CheckedState isChecked);

    [[nodiscard]] auto childItems() const -> QList<QObject *>;
    [[nodiscard]] virtual auto creatables() const -> QStringList;

    [[nodiscard]] virtual auto path() const -> QString;

public slots:
    void toggle();
    void onChildItemAdded();

    virtual void rename(const QString &newName);
    virtual void create(const QString &type, const QString &name);
    virtual void remove();

signals:
    void isCheckedChanged();
    void childItemsChanged();
    void selected();
    void creatablesChanged();

private:
    CheckedState m_isChecked = CheckedState::Checked;

    static auto sortChildren(QObject *o1, QObject *o2) -> bool;

private slots:
    void onChildIsCheckedChanged();
};
