#ifndef AUDIOEDITORFILEBROWSER_H
#define AUDIOEDITORFILEBROWSER_H

#include "../project/audioelement.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAbstractListModel>
#include <QObject>
#include <QQmlApplicationEngine>
#include <utility>

class AudioEditorFile : public QObject
{
    Q_OBJECT

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QStringList, path)
    AUTO_PROPERTY_VAL(int, type)
    AUTO_PROPERTY_VAL2(bool, opened, false)

    Q_PROPERTY(int depth READ depth NOTIFY pathChanged)

public:
    AudioEditorFile(QString name, QStringList path, int type, QObject *parent)
        : QObject(parent), a_name(std::move(name)), a_path(std::move(path)), a_type(type)
    {
    }

    int depth() const
    {
        return path().count();
    }
};

class AudioEditorFileModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
public:
    explicit AudioEditorFileModel(QObject *parent) : QAbstractListModel(parent)
    {
    }

    int rowCount(const QModelIndex &) const override
    {
        return m_items.size();
    }
    QVariant data(const QModelIndex &index, int role) const override;

    QVector<QObject *> elements() const
    {
        return m_items;
    }
    void setElements(const QList<AudioEditorFile *> &elements);

    void clear();
    bool isEmpty() const
    {
        return m_items.isEmpty();
    }

    Q_INVOKABLE QString name() const
    {
        return m_name;
    }
    void setName(const QString &name)
    {
        m_name = name;
    }

    void insert(int index, QObject *item);
    void remove(QObject *item);
    void append(QObject *item);

signals:
    void isEmptyChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject *> m_items = {};
    QString m_name;
};

class AudioEditorFileBrowser : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY_VAL2(AudioElement::Type, type, AudioElement::Type::Music)

public:
    explicit AudioEditorFileBrowser(QQmlApplicationEngine *engine, QObject *parent);

public slots:
    void openFolder(bool open, const QString &folder, const QStringList &path);

private:
    QString m_basePath;

    AudioEditorFileModel *m_fileModel = nullptr;

    void updateFiles();
    void addFiles(const QStringList &path, int index, bool folders);
    void addFilesToModel(const QStringList &files, const QStringList &path, int type, int index);
    void removeElement(const QStringList &path);
    void clearFiles();

private slots:
    void onTypeChanged(AudioElement::Type type);
};

#endif // AUDIOEDITORFILEBROWSER_H
