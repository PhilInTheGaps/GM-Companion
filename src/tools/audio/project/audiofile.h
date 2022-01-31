#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <QObject>
#include <QJsonObject>

#include "thirdparty/propertyhelper/PropertyHelper.h"

class AudioFile : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, url)
    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY(int, source) // 0: file, 1: url, 2: spotify, 3: youtube
    AUTO_PROPERTY(bool, missing)
    Q_PROPERTY(QString printableUrl READ printableUrl NOTIFY urlChanged)

public:
    AudioFile(const QString &url, int source, const QString &title, QObject *parent) :
        QObject(parent), a_url(url), a_title(title), a_source(source) {}

    AudioFile(const QJsonObject &object, QObject *parent = nullptr) : QObject(parent)
    {
        url(object["url"].toString());
        source(object["source"].toInt());
    }

    [[nodiscard]] auto printableUrl() const -> QString {
        return title().isEmpty() ? url() : QString("%1 | %2").arg(title(), url());
    }

    [[nodiscard]] auto toJson() const -> QJsonObject
    {
        return QJsonObject {
            { "url", url() },
            { "source", source() }
        };
    }
};

#endif // AUDIOFILE_H
