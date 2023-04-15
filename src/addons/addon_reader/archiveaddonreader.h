#pragma once

#include "../addon.h"
#include "abstractaddonreader.h"
#include <memory>
#include <quazip.h>

class ArchiveAddonReader : public AbstractAddonReader
{
public:
    explicit ArchiveAddonReader(const Addon &addon);

    auto findAllFiles(const QString &path, const QStringList &filter) -> QStringList override;
    auto readFile(const QString &path) -> QByteArray override;
    auto checkFile(const QString &path) -> bool override;

private:
    void setRootDir();
    [[nodiscard]] auto findAddonJson() const -> QString;

    const Addon &m_addon;
    std::unique_ptr<QuaZip> m_zip;
    QString m_root;
};
