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

    [[nodiscard]] auto depth() const -> qsizetype
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

    [[nodiscard]] auto rowCount(const QModelIndex &) const -> int override
    {
        return static_cast<int>(m_items.size());
    }
    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override;

    [[nodiscard]] auto elements() const -> QList<QObject *>
    {
        return m_items;
    }
    void setElements(const QList<AudioEditorFile *> &elements);

    void clear();
    [[nodiscard]] auto isEmpty() const -> bool
    {
        return m_items.isEmpty();
    }

    [[nodiscard]] Q_INVOKABLE QString name() const
    {
        return m_name;
    }
    void setName(const QString &name)
    {
        m_name = name;
    }

    void insert(int index, QObject *item);
    void remove(const QObject *item);
    void append(QObject *item);

signals:
    void isEmptyChanged();

protected:
    [[nodiscard]] auto roleNames() const -> QHash<int, QByteArray> override;

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
