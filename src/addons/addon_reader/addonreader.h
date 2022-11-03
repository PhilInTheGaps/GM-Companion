#pragma once

#include "../addon.h"
#include "abstractaddonreader.h"

#include <memory>

class AddonReader : public AbstractAddonReader
{
public:
    explicit AddonReader(const Addon &addon);

    auto findAllFiles(const QString &path, const QStringList &filter) -> QStringList override;
    auto readFile(const QString &path) -> QByteArray override;

private:
    std::unique_ptr<AbstractAddonReader> m_reader;
};
