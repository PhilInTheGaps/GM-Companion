#pragma once

#include "../project/audioelement.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>
#include <utility>

class AudioEditorFile : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QStringList, path)
    AUTO_PROPERTY_VAL(int, type)
    AUTO_PROPERTY_VAL2(bool, opened, false)

    Q_PROPERTY(int depth READ depth NOTIFY pathChanged)

public:
    using QObject::QObject;
    explicit AudioEditorFile(QString name, QStringList path, int type, QObject *parent)
        : QObject(parent), a_name(std::move(name)), a_path(std::move(path)), a_type(type)
    {
    }

    int depth() const
    {
        return path().count();
    }
};

class AudioEditorFileBrowserModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
public:
    using QAbstractListModel::QAbstractListModel;

    int rowCount(const QModelIndex &) const override
    {
        return m_items.size();
    }
    QVariant data(const QModelIndex &index, int role) const override;

    QList<QObject *> elements() const
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
    QList<QObject *> m_items = {};
    QString m_name;
};

class AudioEditorFileBrowser : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    AUTO_PROPERTY_VAL2(AudioElement::Type, type, AudioElement::Type::Music)
    Q_PROPERTY(AudioEditorFileBrowserModel *model READ model CONSTANT)

public:
    explicit AudioEditorFileBrowser(QObject *parent);

    [[nodiscard]] auto model() -> AudioEditorFileBrowserModel *
    {
        return &m_fileModel;
    }

public slots:
    void openFolder(bool open, const QString &folder, const QStringList &path);

private:
    QString m_basePath;
    AudioEditorFileBrowserModel m_fileModel;

    void updateFiles();
    void addFiles(const QStringList &path, int index, bool folders);
    void addFilesToModel(const QStringList &files, const QStringList &path, int type, int index);
    void removeElement(const QStringList &path);
    void clearFiles();

private slots:
    void onTypeChanged(AudioElement::Type type);
};
