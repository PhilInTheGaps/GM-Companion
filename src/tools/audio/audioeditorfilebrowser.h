#ifndef AUDIOEDITORFILEBROWSER_H
#define AUDIOEDITORFILEBROWSER_H

#include <QObject>
#include <QAbstractListModel>
#include <QQmlApplicationEngine>

#include "src/settings/settingsmanager.h"

class AudioEditorFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY fileChanged)
    Q_PROPERTY(QStringList path READ path WRITE setPath NOTIFY fileChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY fileChanged)
    Q_PROPERTY(int depth READ depth NOTIFY fileChanged)
    Q_PROPERTY(bool opened READ opened WRITE setOpened NOTIFY openedChanged)

public:
    AudioEditorFile(QString name, QStringList path, int type)
        : m_name(name), m_path(path), m_type(type) {}

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit fileChanged(); }

    QStringList path() const { return m_path; }
    void setPath(QStringList path) { m_path = path; emit fileChanged(); }

    int type() const { return m_type; }
    void setType(int type) { m_type = type; emit fileChanged(); }

    int depth() const { return m_path.count(); }

    bool opened() const { return m_opened; }
    void setOpened(bool opened) { m_opened = opened; emit openedChanged(); }

signals:
    void fileChanged();
    void openedChanged();

private:
    QString m_name;
    QStringList m_path;
    int m_type;
    bool m_opened = false;
};



class AudioEditorFileModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
public:
    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    QVector<QObject*> elements() const { return m_items; }
    void setElements(QList<AudioEditorFile*> elements);

    void clear();
    bool isEmpty() const { return m_items.isEmpty(); }

    Q_INVOKABLE QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }

    void insert(int index, QObject* item);
    void remove(QObject* item);
    void append(QObject* item);

signals:
    void isEmptyChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
    QString m_name;
};




class AudioEditorFileBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)

public:
    explicit AudioEditorFileBrowser(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    void setType(int type);
    int type() const { return m_type; }

    Q_INVOKABLE void addAllFiles();
    Q_INVOKABLE void openFolder(bool open, QString folder, QStringList path);

signals:
    void typeChanged();

private:
    SettingsManager sManager;
    QString m_basePath;
    int m_type;

    QQmlApplicationEngine *qmlEngine = nullptr;
    AudioEditorFileModel *m_fileModel = nullptr;
//    QList<AudioEditorFile*> m_files;

    void updateFiles();
    int addFolders(QStringList path, int index = -1);
    int addFiles(QStringList path, int index = -1);
    void removeElement(QStringList path);
    void clearFiles();
};



#endif // AUDIOEDITORFILEBROWSER_H
