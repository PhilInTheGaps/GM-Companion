#ifndef FOLDERTREE_H
#define FOLDERTREE_H

#include <QObject>

class Folder
{
public:
    Folder(QString name) : m_name(name) {}

    QString getId() const { return m_id; }
    void setId(QString id) { m_id = id; }
    bool hasId() const { return !m_id.isEmpty() ;}

    int getInternalId() const { return m_internalId; }
    void setInternalId(int id) { m_internalId = id; }

    bool isIgnored() const { return m_isIgnored; }
    int ignore(bool ignore = true );
    QString getName() const { return m_name; }

    Folder* getChild(QString name);
    QList<Folder*> children() const { return m_children; }
    void addChild(Folder* folder) { m_children.append(folder); }

    void printSubfolders(int level = 0, bool recursive = true);
    void printMissingSubfolders();

private:
    QList<Folder*> m_children;
    QString m_name;
    QString m_id;
    int m_internalId = -1;
    bool m_isIgnored = false;
};

enum DefaultFolder
{
    Root,
    Music,
    Sounds,
    Radio,
    Audio,
    Maps,
    Shop,
    Characters,
    Notes,
    Resources
};

class FolderTree : public QObject
{
    Q_OBJECT
public:
    explicit FolderTree(QStringList ids, QString rootName, QObject *parent = nullptr);

    Folder* root() const { return m_root; }
    void print();
    void printMissingFolders();

    Folder* getFolder(QString id);
    Folder* getFolder(QString id, Folder *root);
    Folder* getFolder(int internalID);
    Folder* getFolder(int internalID, Folder *root);

    Folder* getDefaultFolder(DefaultFolder f);

    void increaseMissingFolderIDs(int steps = 1) { m_missingFolderIDs += steps; emit missingFolderIDsChanged(m_missingFolderIDs); }
    void decreaseMissingFolderIDs(int steps = 1) { if (m_missingFolderIDs > 0) m_missingFolderIDs -= steps; emit missingFolderIDsChanged(m_missingFolderIDs); }
    int getMissingFolderIDs() const { return m_missingFolderIDs; }

    int getFolderCount() const { return m_folderCount; }
    void increaseFolderCount() { m_folderCount++; }

private:
    Folder *m_root, *m_music, *m_sounds, *m_radio;
    Folder *m_audio, *m_maps, *m_shop, *m_characters;
    Folder *m_notes, *m_resources;

    int m_missingFolderIDs = 0;
    int m_folderCount = 0;

signals:
    void missingFolderIDsChanged(int count);
};

#endif // FOLDERTREE_H
