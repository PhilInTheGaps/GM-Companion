#ifndef AUDIOELEMENT_H
#define AUDIOELEMENT_H

#include <QObject>
#include <QJsonObject>
#include "audioicon.h"
#include "audiofile.h"

class AudioElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(AudioIcon* icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(Type type READ type NOTIFY typeChanged)

public:
    enum class Type
    {
        Music,
        Sound,
        Radio
    };
    Q_ENUM(Type)

    AudioElement(const QString& name, Type type, const QString& path, QObject *parent = nullptr);
    AudioElement(QJsonObject object, Type type, const QString& path, QObject *parent = nullptr);

    QJsonObject toJson() const;

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

    Type type() const { return m_type; }
    static QString typeToString(Type type);

    QString path() const { return m_path; }
    void setPath(const QString &path) { m_path = path; }

    static bool compare(AudioElement *e1, AudioElement *e2);

private:
    QString m_name, m_relativeIconPath, m_path;
    int m_mode = 0;
    Type m_type = Type::Music;
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

#endif // AUDIOELEMENT_H
