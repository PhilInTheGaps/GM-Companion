#pragma once

#include <QByteArray>
#include <QStringList>

class AbstractAddonReader
{
public:
    AbstractAddonReader() = default;
    virtual ~AbstractAddonReader() = default;
    AbstractAddonReader(const AbstractAddonReader &) = delete;
    AbstractAddonReader(AbstractAddonReader &&) = delete;
    AbstractAddonReader &operator=(const AbstractAddonReader &) = delete;
    AbstractAddonReader &operator=(AbstractAddonReader &&) = delete;

    virtual auto findAllFiles(const QString &path, const QStringList &filter) -> QStringList = 0;
    virtual auto readFile(const QString &path) -> QByteArray = 0;
    virtual auto checkFile(const QString &path) -> bool = 0;
};
