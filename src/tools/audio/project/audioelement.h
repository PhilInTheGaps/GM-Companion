#ifndef AUDIOELEMENT_H
#define AUDIOELEMENT_H

#include <QObject>
#include <QJsonObject>
#include "audiofile.h"
#include "../../../models/treeitem.h"

class AudioIcon;

class AudioElement : public TreeItem
{
    Q_OBJECT
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

    bool isCheckable() const override { return true; }

    Type type() const { return m_type; }
    static QString typeToString(Type type);

    QString path() const override { return m_path; }
    void setPath(const QString &path) { m_path = path; }

    static bool compare(AudioElement *e1, AudioElement *e2);

    inline operator QString() const { return QString("%1 (%2)").arg(name(), typeToString(type())); }

private:
    QString m_path;
    int m_mode = 0;
    Type m_type = Type::Music;
    QList<AudioFile*> m_files;
    AudioIcon *m_icon = nullptr;

signals:
    void iconChanged();
    void typeChanged();
    void modeChanged();
    void filesChanged();
};

Q_DECLARE_METATYPE(AudioElement*)
Q_DECLARE_METATYPE(QList<AudioElement*>)

#endif // AUDIOELEMENT_H
