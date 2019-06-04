#ifndef AUDIOELEMENT_H
#define AUDIOELEMENT_H

#include <QObject>
#include <QAbstractListModel>
#include <QUrl>

class AudioElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(bool hasIcon READ hasIcon NOTIFY iconChanged)
    Q_PROPERTY(int type READ type NOTIFY typeChanged)

public:
    AudioElement(QString name, QString icon);
    AudioElement(){}

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    QString icon() const { return m_icon; }
    void setIcon(QString icon) { m_icon = icon; emit iconChanged(); }

    QString relativeIcon() const { return m_relativeIcon; }
    void setRelativeIcon(QString icon) { m_relativeIcon = icon; }
    bool hasIcon() const { return !m_icon.isEmpty(); }

    QStringList files() const { return m_files; }
    void setFiles(QStringList files) { m_files = files; }

    int mode() const { return m_mode; }
    void setMode(int mode) { m_mode = mode; }

    void setExport(bool e) { m_export = e; }
    bool isExport() const { return m_export; }

    virtual int type() const { return -1; }

protected:
    QString m_name, m_icon, m_relativeIcon;
    QStringList m_files;
    int m_mode = 0;
    bool m_export = true;

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

class SoundElement : public AudioElement
{
public:
    SoundElement(QString name);
    int type() const { return 1; }
};

class RadioElement : public AudioElement
{
public:
    RadioElement(QString name, bool local);

    bool local() const { return m_local; }
    void setLocal(bool local) { m_local = local; }

    QUrl url() const { return m_url; }
    void setUrl(QUrl url) { m_url = url; }

    int type() const { return 2; }

private:
    bool m_local;
    QUrl m_url;
};

class SpotifyElement : public AudioElement
{
public:
    SpotifyElement(QString name, QString id);

    QString id() const { return m_id; }
    void setId(QString id) { m_id = id; }

    int iconRequestId() const { return m_iconRequestId; }
    void setIconRequestId(int id) { m_iconRequestId = id; }

    int type() const { return 3; }

private:
    QString m_id;
    int m_iconRequestId = -1;
};

#endif // AUDIOELEMENT_H
