#ifndef AUDIOELEMENT_H
#define AUDIOELEMENT_H

#include <QObject>
#include <QAbstractListModel>
#include <QUrl>
#include <QList>
#include <QImage>

class AudioFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int source READ source WRITE setSource NOTIFY fileChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY fileChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY fileChanged)
    Q_PROPERTY(bool missing READ missing WRITE setMissing NOTIFY fileChanged)

public:
    explicit AudioFile(QString url, int source, QString title = "") :
        m_source(source), m_url(url), m_title(title){}

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
    Q_PROPERTY(QString background READ background WRITE setBackground NOTIFY iconChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY iconChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY iconChanged)

public:
    AudioIcon(QString background, QString title, QString subtitle)
        : m_background(background), m_title(title), m_subtitle(subtitle) {}
    AudioIcon() {}
    virtual ~AudioIcon() {}

    QString background() const { return m_background; }
    void setBackground(QString background) { m_background = background; emit iconChanged(); }

    QString title() const { return m_title; }
    void setTitle(QString title) { m_title = title; emit iconChanged(); }

    QString subtitle() const { return m_subtitle; }
    void setSubtitle(QString subtitle) { m_subtitle = subtitle; emit iconChanged(); }

    QList<QImage> collageIcons() const { return m_collageIcons; }
    bool addCollageIcon(QImage icon);
     void setCollageIcon(QImage icon, int index);

    QStringList collageIconSources() const { return m_collageIconSources; }
    void addCollageIconSource(QString source) { m_collageIconSources.append(source); }

    int lastFileIndex() const { return m_lastFileIndex; }
    void setLastFileIndex(int index) { m_lastFileIndex = index; }

signals:
    void iconChanged();

private:
    QString m_background;
    QString m_title, m_subtitle;
    QList<QImage> m_collageIcons;
    QStringList m_collageIconSources;
    int m_lastFileIndex = 0;
};

class AudioElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(AudioIcon* icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(bool hasIcon READ hasIcon NOTIFY iconChanged)
    Q_PROPERTY(int type READ type NOTIFY typeChanged)

public:
    AudioElement(QString name, AudioIcon *icon);
    AudioElement(){ m_icon = new AudioIcon; }
    ~AudioElement();

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    AudioIcon *icon() const { return m_icon; }
    void setIcon(AudioIcon *icon) { m_icon = icon; emit iconChanged(); }

    QString relativeIcon() const { return m_relativeIcon; }
    void setRelativeIcon(QString icon) { m_relativeIcon = icon; }
    bool hasIcon() const { return !m_icon->background().isEmpty(); }

    void setFiles(QList<AudioFile*> files) { m_files = files; }
    QList<AudioFile*> files() const { return m_files; }

    int mode() const { return m_mode; }
    void setMode(int mode) { m_mode = mode; }

    void setExport(bool e) { m_export = e; }
    bool isExport() const { return m_export; }

    virtual int type() const { return -1; }

protected:
    QString m_name, m_relativeIcon;
    int m_mode = 0;
    bool m_export = true;
    QList<AudioFile*> m_files;
    AudioIcon *m_icon;

signals:
    void nameChanged();
    void iconChanged();
    void typeChanged();
};

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

// Music
class MusicElement : public AudioElement
{
public:
    MusicElement(QString name);
    int type() const { return 0; }
};

// Sound
class SoundElement : public AudioElement
{
public:
    SoundElement(QString name);
    int type() const { return 1; }
};

// Radio
class RadioElement : public AudioElement
{
public:
    RadioElement(QString name);
    int type() const { return 2; }
};

#endif // AUDIOELEMENT_H
