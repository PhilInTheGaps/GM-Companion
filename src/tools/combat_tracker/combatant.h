#ifndef COMBATANT_H
#define COMBATANT_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

class Combatant : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString notes READ notes NOTIFY notesChanged)
    Q_PROPERTY(int ini READ ini NOTIFY iniChanged)
    Q_PROPERTY(int health READ health NOTIFY healthChanged)
    Q_PROPERTY(int priority READ priority NOTIFY priorityChanged)
    Q_PROPERTY(bool delay READ delay NOTIFY delayChanged)

public:
    explicit Combatant(QString name, QString notes, int ini, int health, int priority, QObject *parent);

    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; emit nameChanged(); }

    QString notes() const { return m_notes; }
    void setNotes(const QString &notes) { m_notes = notes; emit notesChanged(); }

    int ini() const { return m_ini; }
    void setIni(int ini) { m_ini = ini; emit iniChanged(); }

    int health() const { return m_health; }
    void setHealth(int health) { m_health = health; emit healthChanged(); }

    int priority() const { return m_priority; }
    void setPriority(int priority) { m_priority = priority; emit priorityChanged(); }

    bool delay() const { return m_delay; }
    void setDelay(bool delay) { m_delay = delay; emit delayChanged(); }

    inline operator QString() const {
        return QString("%1 (Ini: %2 [%3], Health: %4)").arg(
                    name(),
                    QString::number(ini()),
                    QString::number(priority()),
                    QString::number(health()));
    }

signals:
    void nameChanged();
    void notesChanged();
    void iniChanged();
    void healthChanged();
    void delayChanged();
    void priorityChanged();

private:
    QString m_name, m_notes;
    int m_ini, m_health, m_priority;
    bool m_delay = false;
};

// Model for QML
class CombatantListModel : public QAbstractListModel {
    Q_OBJECT
public:
    CombatantListModel(QObject *parent) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    void setElements(QList<Combatant*> elements);
    void clear();

public slots:
    void insert(QObject* item);
    void remove(QObject* item);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
};

#endif // COMBATANT_H
