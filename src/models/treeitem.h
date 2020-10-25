#ifndef TREEITEM_H
#define TREEITEM_H

#include <QObject>

class TreeItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool isOpen READ isOpen WRITE setIsOpen NOTIFY isOpenChanged)
    Q_PROPERTY(bool canToggle READ canToggle WRITE setCanToggle NOTIFY canToggleChanged)
    Q_PROPERTY(bool isCheckable READ isCheckable CONSTANT)
    Q_PROPERTY(int isChecked READ isChecked WRITE setIsChecked NOTIFY isCheckedChanged)
    Q_PROPERTY(QList<QObject*> childItems READ childItems NOTIFY childItemsChanged)
    Q_PROPERTY(QStringList creatables READ creatables NOTIFY creatablesChanged)

public:
    explicit TreeItem(QString name, int depth, bool canToggle = true, QObject *parent = nullptr);

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; emit nameChanged(); }

    /// Whether children are shown or not.
    bool isOpen() const { return m_isOpen; }
    void setIsOpen(bool isOpen) { m_isOpen = isOpen; emit isOpenChanged(); emit childItemsChanged(); }

    /// Whether the item can be toggled to display any children.
    bool canToggle() const { return m_canToggle; }
    void setCanToggle(bool canToggle) { m_canToggle = canToggle; emit canToggleChanged(); }

    /// Whether a checkbox should be displayed.
    virtual bool isCheckable() const { return false; }
    int isChecked() const { return m_isChecked; }
    void setIsChecked(int isChecked);

    QList<QObject *> childItems() const;
    virtual QStringList creatables() const { return {}; }

    /// Sorting priority. Smaller means higher priority.
    int priority() const { return m_priority; }
    void setPriority(int priority) { m_priority = priority; }

    virtual QString path() const;
    Q_INVOKABLE int depth() const { return m_depth; }

public slots:
    void toggle() { if (canToggle()) { setIsOpen(!m_isOpen); } emit selected(); }
    void childItemAdded() { emit childItemsChanged(); }

    virtual void rename(const QString& newName) { setName(newName); }
    virtual void create(const QString& type, const QString& name) { Q_UNUSED(type); Q_UNUSED(name); }
    virtual void remove() { this->deleteLater(); }

signals:
    void nameChanged();
    void isOpenChanged();
    void isCheckedChanged();
    void childItemsChanged();
    void canToggleChanged();
    void selected();
    void creatablesChanged();

private:
    QString m_name;
    bool m_isOpen = false;
    bool m_canToggle = true;
    int m_isChecked = 2;
    int m_depth = 0;
    int m_priority = INT_MAX;

    static bool sortChildren(QObject *o1, QObject* o2);

private slots:
    void onChildIsCheckedChanged();

};

#endif // TREEITEM_H
