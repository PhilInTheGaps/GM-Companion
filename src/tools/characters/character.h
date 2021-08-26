#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class CharacterFile : public QObject
{
    Q_OBJECT

    READONLY_PROPERTY(QString, name)
    READONLY_PROPERTY(QString, path)
    AUTO_PROPERTY(QByteArray, data)

public:
    explicit CharacterFile(const QString &name, QString path, QObject *parent)
        : QObject(parent), a_name(name), a_path(path) {}
};

class Character : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(QString, folder)

public:
    explicit Character(const QString &name, QObject *parent);

    int type() const;

    QList<CharacterFile*> files() const { return m_files; }
    void setFiles(QList<CharacterFile*> files) { m_files = files; }
    void addFile(CharacterFile *file) { m_files.append(file); }

    void loadFiles();
    void loadFileList();
    void loadFileData(int index);

signals:
    void fileListLoaded(const QList<CharacterFile*> &files);
    void fileDataLoaded(int index, const QByteArray &data);

private:
    QList<CharacterFile*> m_files;
    bool m_wasLoaded = false;

};

#endif // CHARACTER_H
