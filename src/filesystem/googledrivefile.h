#ifndef GOOGLEDRIVEFILE_H
#define GOOGLEDRIVEFILE_H

#include <QObject>
#include <QJsonObject>
#include <QDateTime>
#include <QTemporaryFile>

enum GoogleDriveFileStatus {
    Valid,
    Invalid,
    NotFound,
    OtherError,
    Refreshing
};

class GoogleDriveFile;

struct GoogleDriveFileReply {
    GoogleDriveFileReply(GoogleDriveFile *file, GoogleDriveFileStatus status, QString nextFile = "") : file(file), status(status), nextFile(nextFile) {}
    GoogleDriveFile *file;
    GoogleDriveFileStatus status;
    QString nextFile;
};

class GoogleDriveFile : public QObject
{
    Q_OBJECT
public:
    GoogleDriveFile(QString name, QString id, bool isFolder, GoogleDriveFile *parent = nullptr);
    GoogleDriveFile(QJsonObject fileObject, GoogleDriveFile *parent = nullptr);
    ~GoogleDriveFile();

    GoogleDriveFileReply getFile(QString path);
    GoogleDriveFileReply getFile(QStringList path);
    bool containsChild(const QString& name);

    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    QString id() const { return m_id; }
    void setId(const QString &id) { m_id = id; }

    bool isFolder() const { return m_isFolder; }
    void setIsFolder(const bool &isFolder) { m_isFolder = isFolder; }

    QList<GoogleDriveFile*> children() const { return m_children; }
    void addChild(GoogleDriveFile *child);
    void removeChild(GoogleDriveFile *child) { m_children.removeOne(child); }

    void receivedIndex();
    bool isUpToDate() const;

    void setRefreshed() { m_refreshTime = QDateTime::currentDateTime(); }
    void setRefreshing() { m_isRefreshing = true; }

    void writeData(const QByteArray& data);
    QByteArray readData();

private:
    QString m_name, m_id;
    bool m_isFolder = false, m_isRefreshing = false, m_hasData = false;
    QList<GoogleDriveFile*> m_children;
    QDateTime m_refreshTime;
    QTemporaryFile m_file;
};

#endif // GOOGLEDRIVEFILE_H
