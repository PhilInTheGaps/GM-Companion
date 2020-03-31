#ifndef ADDONELEMENTMANAGER_H
#define ADDONELEMENTMANAGER_H

#include <QObject>
#include "audioelement.h"

class AddonElementManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList spotifyFolders READ spotifyFolders NOTIFY spotifyFoldersChanged)
    Q_PROPERTY(QStringList elementNames READ elements NOTIFY elementsChanged)

public:
    explicit AddonElementManager(QObject *parent = nullptr);

    QStringList spotifyFolders() const { return m_spotifyFolders; }
    Q_INVOKABLE void setFolder(QString folder);

    QStringList elements() const { return m_spotifyNames; }
    Q_INVOKABLE void setAddElement(int index, bool add);

    Q_INVOKABLE void resetChecked();
    Q_INVOKABLE void addElements(bool subscenario = false, int scenarioIndex = 0);

private:
    void findAddons();
    QStringList m_addonPaths;
    QStringList m_spotifyFiles;

    void findSpotifyFolders();
    QStringList m_spotifyFolders;
    QString m_currentSpotifyFolder;

    void findSpotifyPlaylists();
    QList<AudioElement*> m_spotifyElements;
    QStringList m_spotifyNames;

signals:
    void spotifyFoldersChanged();
    void elementsChanged();
    void exportElements(QList<AudioElement*>, bool subscenario, int scenarioIndex);
};

#endif // ADDONELEMENTMANAGER_H
