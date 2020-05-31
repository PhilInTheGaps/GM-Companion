#ifndef TREEITEM_H
#define TREEITEM_H

#include <QObject>

class TreeItem : public QObject
{
    Q_OBJECT

public:
    explicit TreeItem(const QString &name, int depth, bool canToggle = true, QObject *parent = nullptr);

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; emit nameChanged(); }

    Q_PROPERTY(bool isOpen READ isOpen WRITE setIsOpen NOTIFY isOpenChanged)
    bool isOpen() const { return m_isOpen; }
    void setIsOpen(bool isOpen) { m_isOpen = isOpen; emit isOpenChanged(); emit childItemsChanged(); }

    Q_PROPERTY(QList<QObject*> childItems READ childItems NOTIFY childItemsChanged)
    QList<QObject *> childItems() const;

    Q_PROPERTY(bool canToggle READ canToggle WRITE setCanToggle NOTIFY canToggleChanged)
    bool canToggle() const { return m_canToggle; }
    void setCanToggle(bool canToggle) { m_canToggle = canToggle; emit canToggleChanged(); }

    Q_PROPERTY(QStringList creatables READ creatables NOTIFY creatablesChanged)
    virtual QStringList creatables() const { return {}; }

    int priority() const { return m_priority; }
    void setPriority(int priority) { m_priority = priority; }

    QString path() const;
    Q_INVOKABLE int depth() const { return m_depth; }

public slots:
    void toggle() { setIsOpen(!m_isOpen); emit selected(); }
    void childItemAdded() { emit childItemsChanged(); }
    virtual void rename(const QString& newName) { setName(newName); }
    virtual void create(const QString& /*type*/, const QString& /*name*/) {}
    virtual void remove() { this->deleteLater(); }

signals:
    void nameChanged();
    void isOpenChanged();
    void childItemsChanged();
    void canToggleChanged();
    void selected();
    void creatablesChanged();

private:
    QString m_name;
    bool m_isOpen = false;
    bool m_canToggle = true;
    int m_depth = 0;
    int m_priority = INT_MAX;
};

#endif // TREEITEM_H
