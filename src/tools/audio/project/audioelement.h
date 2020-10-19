#ifndef AUDIOELEMENT_H
#define AUDIOELEMENT_H

#include <QObject>
#include <QJsonObject>
#include "audiofile.h"

class AudioIcon;

class AudioElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QObject* icon READ iconObject NOTIFY iconChanged)
    Q_PROPERTY(Type type READ type NOTIFY typeChanged)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged)

public:
    enum Type
    {
        Music = 0,
        Sound = 1,
        Radio = 2
    };
    Q_ENUM(Type)

    AudioElement(const QString& name, Type type, const QString& path, QObject *parent);
    AudioElement(const QJsonObject &object, Type type, const QString& path, QObject *parent);

    QJsonObject toJson() const;

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; emit nameChanged(); }

    AudioIcon *icon() const { return m_icon; }
    QObject *iconObject() const;
    void setIcon(AudioIcon *icon) { m_icon = icon; emit iconChanged(); }

    QList<AudioFile*> files() const { return m_files; }
    void setFiles(QList<AudioFile*> files);
    bool addFile(AudioFile *file);
    bool removeFile(int index);
    bool moveFile(int from, int steps);

    int mode() const { return m_mode; }
    void setMode(int mode) { m_mode = mode; emit modeChanged(); }

    void setExport(bool e) { m_export = e; }
    bool isExport() const { return m_export; }

    Type type() const { return m_type; }
    static QString typeToString(Type type);

    QString path() const { return m_path; }
    void setPath(const QString &path) { m_path = path; }

    static bool compare(AudioElement *e1, AudioElement *e2);

    inline operator QString() const { return QString("%1 (%2)").arg(name(), typeToString(type())); }

private:
    QString m_name, m_path;
    int m_mode = 0;
    Type m_type = Type::Music;
    bool m_export = true;
    QList<AudioFile*> m_files;
    AudioIcon *m_icon = nullptr;

signals:
    void nameChanged();
    void iconChanged();
    void typeChanged();
    void modeChanged();
    void filesChanged();
};

Q_DECLARE_METATYPE(AudioElement*)
Q_DECLARE_METATYPE(QList<AudioElement*>)

#endif // AUDIOELEMENT_H
