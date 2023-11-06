#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <utility>

class CharacterFile : public QObject
{
    Q_OBJECT

    READONLY_PROPERTY(QString, name)
    READONLY_PROPERTY(QString, path)
    AUTO_PROPERTY(QByteArray, data)

public:
    explicit CharacterFile(QString name, QString path, QObject *parent)
        : QObject(parent), a_name(std::move(name)), a_path(std::move(path))
    {
    }
};

class Character : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, folder)

public:
    explicit Character(const QString &name, QObject *parent);

    [[nodiscard]] auto type() const -> int;

    [[nodiscard]] auto files() const -> QList<CharacterFile *>
    {
        return m_files;
    }
    void setFiles(QList<CharacterFile *> files)
    {
        m_files = std::move(files);
    }
    void addFile(CharacterFile *file)
    {
        m_files.append(file);
    }

    void loadFiles();
    void loadFileList();
    void loadFileData(qsizetype index);

signals:
    void fileListLoaded(const QList<CharacterFile *> &files);
    void fileDataLoaded(int index, const QByteArray &data);

private:
    QList<CharacterFile *> m_files;
    bool m_wasLoaded = false;
};
