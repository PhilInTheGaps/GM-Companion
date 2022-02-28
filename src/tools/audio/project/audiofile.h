#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <QObject>
#include <QJsonObject>

#include "thirdparty/propertyhelper/PropertyHelper.h"

class AudioFile : public QObject
{
    Q_OBJECT

public:
    enum AudioFileSource {
        Unknown = -1,
        File = 0,
        Web = 1,
        Spotify = 2,
        Youtube = 3
    };
    Q_ENUM(AudioFileSource)

    AudioFile(const QString &url, AudioFileSource source, const QString &title, QObject *parent);
    AudioFile(const QJsonObject &object, QObject *parent = nullptr);

    [[nodiscard]] auto printableUrl() const -> QString;
    [[nodiscard]] auto toJson() const -> QJsonObject;

    Q_PROPERTY(QString displayName READ displayName NOTIFY titleChanged)
    [[nodiscard]] auto displayName() const -> QString;

    AUTO_PROPERTY(QString, url)
    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY(AudioFileSource, source)
    AUTO_PROPERTY(bool, missing)
    Q_PROPERTY(QString printableUrl READ printableUrl NOTIFY urlChanged)
};

#endif // AUDIOFILE_H
