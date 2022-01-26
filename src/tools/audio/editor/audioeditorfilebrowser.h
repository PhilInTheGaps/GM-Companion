#ifndef AUDIOEDITORFILEBROWSER_H
#define AUDIOEDITORFILEBROWSER_H

#include <QObject>
#include <QAbstractListModel>
#include <QQmlApplicationEngine>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AudioEditorFile : public QObject
{
    Q_OBJECT

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QStringList, path)
    AUTO_PROPERTY(int, type)
    AUTO_PROPERTY(bool, opened)

    Q_PROPERTY(int depth READ depth NOTIFY pathChanged)

public:
    AudioEditorFile(QString name, QStringList path, int type, QObject *parent)
        : QObject(parent), a_name(name), a_path(path), a_type(type) {}

    int depth() const { return path().count(); }
};



class AudioEditorFileModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
public:
    explicit AudioEditorFileModel(QObject *parent) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    QVector<QObject*> elements() const { return m_items; }
    void setElements(const QList<AudioEditorFile *> &elements);

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
    explicit AudioEditorFileBrowser(QQmlApplicationEngine *engine, QObject *parent);

    void setType(int type);
    int type() const { return m_type; }

public slots:
    void openFolder(bool open, const QString& folder, const QStringList& path);

signals:
    void typeChanged();

private:
    QString m_basePath;
    int m_type;

    QQmlApplicationEngine *qmlEngine = nullptr;
    AudioEditorFileModel *m_fileModel = nullptr;

    void updateFiles();
    void addFiles(const QStringList& path, int index, bool folders);
    void addFilesToModel(const QStringList& files, const QStringList& path, int type, int index);
    void removeElement(const QStringList& path);
    void clearFiles();
};



#endif // AUDIOEDITORFILEBROWSER_H
