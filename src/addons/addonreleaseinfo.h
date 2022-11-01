#pragma once

#include <QString>
#include <utility>

struct AddonReleaseInfo
{
    explicit AddonReleaseInfo() = default;
    explicit AddonReleaseInfo(QString id, QString name, QString shortName, QString version,
                              QString author, QString description, QString downloadUrl)
        : id(std::move(id)),
          name(std::move(name)),
          shortName(std::move(shortName)),
          version(std::move(version)),
          author(std::move(author)),
          description(std::move(description)),
          downloadUrl(std::move(downloadUrl)) {}

    QString id;
    QString name;
    QString shortName;
    QString version;
    QString author;
    QString description;
    QString downloadUrl;
};
