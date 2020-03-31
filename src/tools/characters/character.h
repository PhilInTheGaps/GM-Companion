#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>

class CharacterFile
{
public:
    CharacterFile(QString name, QString path) : m_name(name), m_path(path) {}
    ~CharacterFile() {}

    QString name() const { return m_name; }
    QString path() const { return m_path; }

    QByteArray data() const { return m_data; }
    void setData(QByteArray data) { m_data = data; }

private:
    QString m_name, m_path;
    QByteArray m_data;
};

class Character : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Character(QString name, QObject *parent = nullptr);

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }

    int type() const;

    QList<CharacterFile> files() const { return m_files; }
    void setFiles(QList<CharacterFile> files) { m_files = files; }
    void addFile(CharacterFile file) { m_files.append(file); }

    void loadFiles();
    void loadFileList();
    void loadFileData(int index);

    void setFolder(QString folder) { m_folder = folder; }

signals:
    void nameChanged();
    void fileListLoaded(QList<CharacterFile> files);
    void fileDataLoaded(int index, QByteArray data);

private:
    QString m_name, m_folder;
    int m_requestId = -1;
    QList<CharacterFile> m_files;
    bool m_wasLoaded = false;

};

#endif // CHARACTER_H
