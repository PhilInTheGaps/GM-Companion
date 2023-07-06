#include "converterprojectupgrader.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmConverterUpgrader, "gm.converter.project.upgrader")

ConverterProjectUpgrader::ConverterProjectUpgrader() : AbstractProjectUpgraderINI(OLD_VERSION)
{
}

auto ConverterProjectUpgrader::run() -> QByteArray
{
    if (!isProjectCompatible()) return {};

    qCDebug(gmConverterUpgrader()) << "Converting project to newest version ...";

    const auto ini = this->ini();
    const auto categoryNames = ini->childGroups();

    QJsonArray categories;

    for (const auto &categoryName : categoryNames)
    {
        categories << convertCategory(categoryName);
    }

    const QJsonObject projectNew{{"name", findProjectName()}, {"version", NEW_VERSION}, {"categories", categories}};
    return QJsonDocument(projectNew).toJson();
}

auto ConverterProjectUpgrader::convertCategory(const QString &category) const -> QJsonObject
{
    const auto ini = this->ini();
    QJsonArray units;

    const auto length = ini->beginReadArray(category);

    for (int i = 0; i < length; i++)
    {
        ini->setArrayIndex(i);
        units << QJsonObject{{"name", ini->value("name").toString()}, {"value", ini->value("refUnits").toDouble()}};
    }

    ini->endArray();

    return QJsonObject{{"name", categoryNameFromGroup(category)}, {"units", units}};
}

auto ConverterProjectUpgrader::findProjectName() const -> QString
{
    const auto ini = this->ini();

    const auto categoryNames = ini->childGroups();
    for (const auto &categoryName : categoryNames)
    {
        const auto length = ini->beginReadArray(categoryName);

        for (int i = 0; i < length; i++)
        {
            ini->setArrayIndex(i);
            auto projectName = ini->value("category"_L1).toString();
            if (!projectName.isEmpty())
            {
                ini->endArray();
                return projectName;
            }
        }

        ini->endArray();
    }

    return u"Custom"_s;
}

auto ConverterProjectUpgrader::categoryNameFromGroup(const QString &group) -> QString
{
    if (group.endsWith("Units"_L1))
    {
        return group.left(group.length() - "Units"_L1.length());
    }

    return group;
}
