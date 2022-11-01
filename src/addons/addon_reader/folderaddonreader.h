#pragma once

#include "../addon.h"
#include "abstractaddonreader.h"

class FolderAddonReader : public AbstractAddonReader
{
public:
    FolderAddonReader(const Addon &addon);

    auto findAllFiles(const QString &path, const QStringList &filter) -> QStringList override;
    auto readFile(const QString &path) -> QByteArray override;

private:
    const Addon &m_addon;
};
