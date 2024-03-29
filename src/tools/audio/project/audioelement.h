#pragma once

#include "../thumbnails/audiothumbnail.h"
#include "audiofile.h"
#include "models/treeitem.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class AudioScenario;

class AudioElement : public TreeItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

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
    ~AudioElement() override = default;

    [[nodiscard]] auto toJson() const -> QJsonObject;

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

    static auto compare(const AudioElement *e1, const AudioElement *e2) -> bool;

    explicit inline operator QString() const
    {
        return QStringLiteral("%1 (%2)").arg(name(), typeToString(type()));
    }

    static auto typeToString(Type type) -> QString;
    static auto typeToSettings(AudioElement::Type type) -> QString;
    READONLY_PROPERTY(Type, type)

    AUTO_PROPERTY_VAL(AudioThumbnail *, thumbnail)
    AUTO_PROPERTY_VAL2(AudioElement::Mode, mode, Mode::RandomList)

private:
    QString m_path;
    QList<AudioFile *> m_files;

signals:
    void filesChanged();
};

Q_DECLARE_METATYPE(AudioElement *)
