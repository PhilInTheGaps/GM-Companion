#pragma once

#include "../addon.h"
#include "abstractaddonreader.h"

class FolderAddonReader : public AbstractAddonReader
{
public:
    explicit FolderAddonReader(const Addon &addon);

    auto findAllFiles(const QString &path, const QStringList &filter) -> QStringList override;
    auto readFile(const QString &path) -> QByteArray override;
    auto checkFile(const QString &path) -> bool override;

private:
    const Addon &m_addon;
};
