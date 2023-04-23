#ifndef AUDIOELEMENT_H
#define AUDIOELEMENT_H

#include "audiofile.h"
#include "models/treeitem.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>

class AudioThumbnail;
class AudioScenario;

class AudioElement : public TreeItem
{
    Q_OBJECT

public:
    enum class Type
    {
        Music = 0,
        Sound = 1,
        Radio = 2
    };
    Q_ENUM(Type)

    enum class Mode
    {
        RandomList = 0,
        Random = 1,
        ListLoop = 2,
        ListOnce = 3
    };
    Q_ENUM(Mode)

    explicit AudioElement(const QString &name, Type type, const QString &path, AudioScenario *parent);
    explicit AudioElement(const QJsonObject &object, Type type, const QString &path, AudioScenario *parent);
    explicit AudioElement(const AudioElement &other);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    Q_PROPERTY(QObject *thumbnail READ thumnailObject NOTIFY thumbnailChanged)
    [[nodiscard]] auto thumbnail() const -> AudioThumbnail *
    {
        return m_thumbnail;
    }
    [[nodiscard]] auto thumnailObject() const -> QObject *;
    void setThumbnail(AudioThumbnail *icon)
    {
        m_thumbnail = icon;
        emit thumbnailChanged();
    }

    [[nodiscard]] auto files() const -> QList<AudioFile *>
    {
        return m_files;
    }
    void setFiles(QList<AudioFile *> files);
    auto addFile(AudioFile *file) -> bool;
    auto removeFile(int index) -> bool;
    auto moveFile(int from, int steps) -> bool;

    [[nodiscard]] auto isCheckable() const -> bool override
    {
        return true;
    }

    [[nodiscard]] auto path() const -> QString override
    {
        return m_path;
    }
    void setPath(const QString &path)
    {
        m_path = path;
    }

    static auto compare(AudioElement *e1, AudioElement *e2) -> bool;

    inline operator QString() const
    {
        return QString("%1 (%2)").arg(name(), typeToString(type()));
    }

    static auto typeToString(Type type) -> QString;
    static auto typeToSettings(AudioElement::Type type) -> QString;
    READONLY_PROPERTY(Type, type)

    AUTO_PROPERTY(AudioElement::Mode, mode)

private:
    QString m_path;
    QList<AudioFile *> m_files;
    AudioThumbnail *m_thumbnail = nullptr;

signals:
    void thumbnailChanged();
    void filesChanged();
};

Q_DECLARE_METATYPE(AudioElement *)
Q_DECLARE_METATYPE(QList<AudioElement *>)

#endif // AUDIOELEMENT_H
