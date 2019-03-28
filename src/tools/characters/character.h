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

private:
    QString m_name, m_path;
};

class Character : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Character(QString name, QObject *parent = nullptr);

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }

    QList<CharacterFile> files() const { return m_files; }
    void setFiles(QList<CharacterFile> files) { m_files = files; }
    void addFile(CharacterFile file) { m_files.append(file); }

    int type() const;

    QString cloudId() const { return m_cloudId; }
    void setCloudId(QString id) { m_cloudId = id; }

    int requestId() const { return m_requestId; }
    void setRequestId(int id) { m_requestId = id; }

signals:
    void nameChanged();

private:
    QString m_name, m_cloudId;
    int m_requestId = -1;
    QList<CharacterFile> m_files;

};

#endif // CHARACTER_H
