#include "abstractprojectupgraderini.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAbstractProjectUpgraderINI, "gm.common.projects.upgrader.ini")

AbstractProjectUpgraderINI::AbstractProjectUpgraderINI(int version) : AbstractProjectUpgrader(version)
{
}

void AbstractProjectUpgraderINI::parse(const QByteArray &data)
{
    if (!m_file.open())
    {
        qCCritical(gmAbstractProjectUpgraderINI()) << "Could not open a temporary file";
        return;
    }

    m_file.write(data);
    m_file.close();

    m_ini = std::make_shared<QSettings>(m_file.fileName(), QSettings::IniFormat);
}

auto AbstractProjectUpgraderINI::isProjectCompatible() const -> bool
{
    if (!m_ini) return false;

    const auto version = m_ini->value("version"_L1, 0).toInt();

    // does the file contain any keys?
    // if not it is probably of a different format
    if (const auto entries = m_ini->allKeys(); entries.isEmpty()) return false;

    return version == this->version();
}
