#ifndef EFFECTTOOL_H
#define EFFECTTOOL_H

#include <QObject>
#include <QStringList>
#include "src/settings/settingsmanager.h"
#include "effect.h"

class EffectTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList effectTypes READ effectTypes NOTIFY effectTypesChanged)
    Q_PROPERTY(QStringList addons READ addons NOTIFY addonsChanged)

public:
    explicit EffectTool(QObject *parent = nullptr);

    Q_INVOKABLE void loadEffects();
    QStringList effectTypes() const { return m_effectTypes; }

    Q_INVOKABLE void loadAddons();
    Q_INVOKABLE void setCurrentAddon(QString addon) { m_currentAddon = m_addons[m_addonNames.indexOf(addon)]; loadEffects(); }
    QStringList addons() const { return m_addonNames; }

    Q_INVOKABLE QString getIcon(int index);

    Q_INVOKABLE QString randomEffect(QString effectType);

private:
    SettingsManager sManager;
    QStringList m_addons;
    QStringList m_addonNames;
    QString m_currentAddon;
    QStringList m_addonPaths;
    QStringList m_effectTypes;
    QList<Effect*> m_effectList;

signals:
    void effectTypesChanged();
    void addonsChanged();
};

#endif // EFFECTTOOL_H
