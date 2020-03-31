#ifndef AUDIOELEMENT_H
#define AUDIOELEMENT_H

#include <QObject>
#include <QAbstractListModel>
#include <QUrl>
#include <QList>
#include <QPixmap>
#include <QJsonObject>

class AudioFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int source READ source WRITE setSource NOTIFY fileChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY fileChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY fileChanged)
    Q_PROPERTY(bool missing READ missing WRITE setMissing NOTIFY fileChanged)

public:
    AudioFile(QString url, int source, QString title, QObject *parent) :
        QObject(parent), m_source(source), m_url(url), m_title(title){}
    AudioFile(QJsonObject object, QObject *parent = nullptr);

    friend void swap(AudioFile & first, AudioFile & second) {
        // d_ptr swap doesn't take care of parentage
        QObject * firstParent = first.parent();
        QObject * secondParent = second.parent();
        first.setParent(nullptr);
        second.setParent(nullptr);
        first.d_ptr.swap(second.d_ptr);
        second.setParent(firstParent);
        first.setParent(secondParent);
    }

    AudioFile(const AudioFile &other) : QObject(other.parent()) {
        m_url = other.m_url;
        m_source = other.m_source;
        m_title = other.m_title;
    }

    AudioFile & operator=(AudioFile other) {
        swap(*this, other);
        return *this;
    }

    int source() const { return m_source; }
    void setSource(int source) { m_source = source; emit fileChanged(); }

    QString url() const { return m_url; }
    void setUrl(QString url) { m_url = url; emit fileChanged(); }

    QString title() const { return m_title; }
    void setTitle(QString title) { m_title = title; emit fileChanged(); }

    bool missing() const { return m_missing; }
    void setMissing(bool missing) { m_missing = missing; emit fileChanged(); }


private:
    int m_source = -1;
    QString m_url, m_title;
    bool m_missing = false;

signals:
    void fileChanged();
};


class AudioFileModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
public:
    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;

    QVector<QObject*> elements() const { return m_items; }
    void setElements(QList<AudioFile*> elements);

    void clear();
    bool isEmpty() const { return m_items.isEmpty(); }

    Q_INVOKABLE QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }

    void insert(int index, QObject* item);
    void remove(QObject* item);
    void append(QObject* item);
    bool moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationChild);

signals:
    void isEmptyChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
    QString m_name;
};

class AudioIcon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imageId READ imageId WRITE setImageId NOTIFY iconChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY iconChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY iconChanged)

public:
    AudioIcon(QString image, QString title, QString subtitle)
        : m_imageId(image), m_title(title), m_subtitle(subtitle) {}
    AudioIcon(QString imageId) : m_imageId(imageId) {}
    AudioIcon() {}

    QString imageId() const { return m_imageId; }
    void setImageId(QString image) { m_imageId = image; emit iconChanged(); }

    QString title() const { return m_title; }
    void setTitle(QString title) { m_title = title; emit iconChanged(); }

    QString subtitle() const { return m_subtitle; }
    void setSubtitle(QString subtitle) { m_subtitle = subtitle; emit iconChanged(); }

    QList<QPair<QString, QPixmap>> collageIcons() const { return m_collageIcons; }
    bool addCollageIcon(const QPair<QString, QPixmap> icon);
    void setCollageIcon(const QPair<QString, QPixmap> icon, int index);

    int lastFileIndex() const { return m_lastFileIndex; }
    void setLastFileIndex(int index) { m_lastFileIndex = index; }
    void update() { emit iconChanged(); }

signals:
    void iconChanged();

private:
    QString m_imageId, m_title, m_subtitle;
    QList<QPair<QString, QPixmap>> m_collageIcons;
    int m_lastFileIndex = 0;
};

class AudioElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(AudioIcon* icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(int type READ type NOTIFY typeChanged)

public:
    AudioElement(QString name, int type, QString path);
    AudioElement(QJsonObject object, int type, QString path, QObject *parent = nullptr);
    ~AudioElement();

    QJsonObject toJson();

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    AudioIcon *icon() const { return m_icon; }
    void setIcon(AudioIcon *icon) { m_icon = icon; emit iconChanged(); }

    QString relativeIconPath() const { return m_relativeIconPath; }
    void setRelativeIcon(QString icon) { m_relativeIconPath = icon; }

    void setFiles(QList<AudioFile*> files) { m_files = files; }
    QList<AudioFile*> files() const { return m_files; }

    int mode() const { return m_mode; }
    void setMode(int mode) { m_mode = mode; }

    void setExport(bool e) { m_export = e; }
    bool isExport() const { return m_export; }

    int type() const { return m_type; }

    QString path() const { return m_path; }
    void setPath(const QString &path) { m_path = path; }

private:
    QString m_name, m_relativeIconPath, m_path;
    int m_mode = 0, m_type = -1;
    bool m_export = true;
    QList<AudioFile*> m_files;
    AudioIcon *m_icon = nullptr;

signals:
    void nameChanged();
    void iconChanged();
    void typeChanged();
};

Q_DECLARE_METATYPE(AudioElement*)
Q_DECLARE_METATYPE(QList<AudioElement*>)

// Model for QML
class AudioElementModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
public:
    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;
    void setElements(QList<AudioElement*> elements);
    void clear();
    bool isEmpty() const { return m_items.isEmpty(); }

    Q_INVOKABLE QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }

public slots:
    void insert(QObject* item);
    void remove(QObject* item);

signals:
    void isEmptyChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
    QString m_name;
};

class AudioElementModelModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
public:
    int rowCount(const QModelIndex&) const override { return m_items.size(); }
    QVariant data(const QModelIndex& index, int role) const override;
    void setElements(QList<AudioElementModel *> elements);
    void clear();
    bool isEmpty() const { return m_items.isEmpty(); }
    AudioElementModel* element(int index) { if (m_items.size() > 0) return static_cast<AudioElementModel*>(m_items[index]); else return nullptr; }

public slots:
    void insert(QObject* item);
    void append(QObject* item);
    void remove(QObject* item);

signals:
    void isEmptyChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QObject*> m_items = {};
};

#endif // AUDIOELEMENT_H
