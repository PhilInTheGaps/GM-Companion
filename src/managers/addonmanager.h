#ifndef ADDONMANAGER_H
#define ADDONMANAGER_H

#include <QObject>
#include <QQmlContext>

class AddonManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList addonNames READ addonNames NOTIFY addonListChanged)

public:
    explicit AddonManager(QObject *parent = nullptr);

    Q_INVOKABLE void updateAddonList();
    QStringList addonNames() const { return m_addonNames; }
    Q_INVOKABLE QStringList getAddonDescriptions() const { return m_addonDescriptions; }
    Q_INVOKABLE QStringList getAddonPathNames() const { return m_addonPathNames; }
    Q_INVOKABLE QList<bool> getAddonEnabledList() const { return m_addonEnabledList; }

signals:
    void addonListChanged();

private:
    QStringList m_addonNames;
    QStringList m_addonPathNames;
    QStringList m_addonDescriptions;
    QList<bool> m_addonEnabledList;
};

#endif // ADDONMANAGER_H
