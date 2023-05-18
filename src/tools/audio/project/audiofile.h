#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <QJsonObject>
#include <QObject>

#include "thirdparty/propertyhelper/PropertyHelper.h"

class AudioFile : public QObject
{
    Q_OBJECT

public:
    enum class Source
    {
        Unknown = -1,
        File = 0,
        Web = 1,
        Spotify = 2,
        Youtube = 3
    };
    Q_ENUM(Source)

    explicit AudioFile(const QString &url, Source source, const QString &title, QObject *parent);
    explicit AudioFile(const QJsonObject &object, QObject *parent = nullptr);
    explicit AudioFile(const AudioFile &other);

    [[nodiscard]] auto printableUrl() const -> QString;
    [[nodiscard]] auto toJson() const -> QJsonObject;

    Q_PROPERTY(QString displayName READ displayName NOTIFY titleChanged)
    [[nodiscard]] auto displayName() const -> QString;

    AUTO_PROPERTY(QString, url)
    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY_VAL(Source, source)
    AUTO_PROPERTY_VAL2(bool, missing, false)
    Q_PROPERTY(QString printableUrl READ printableUrl NOTIFY urlChanged)
};

#endif // AUDIOFILE_H
