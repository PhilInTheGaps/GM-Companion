#pragma once

#include "../addon.h"
#include "abstractaddonreader.h"

#include <memory>

class AddonReader : public AbstractAddonReader
{
public:
    enum class Feature
    {
        None,
        Audio,
        Names,
        Units
    };
    Q_DECLARE_FLAGS(Features, Feature)

    explicit AddonReader(const Addon &addon);

    auto findAllFiles(const QString &path, const QStringList &filter) -> QStringList override;
    auto readFile(const QString &path) -> QByteArray override;
    auto checkFile(const QString &path) -> bool override;

    auto getFeatures() -> Features;

private:
    std::unique_ptr<AbstractAddonReader> m_reader;
};
