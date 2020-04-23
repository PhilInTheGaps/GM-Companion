#include "namegenerator.h"
#include "settings/settingsmanager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QQmlContext>

NameGenerator::NameGenerator(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Name Generator ...";

    engine->rootContext()->setContextProperty("name_generator", this);
}

void NameGenerator::updateCategories()
{
    qDebug() << "Updating Name Categories ...";

    m_categories.clear();
    m_categoryPaths.clear();

    // Generic Default Names
    m_categories.append("Generic");
    m_categoryPaths.append(":/names/Generic");

    //    // Custom Names
    //    for (QString folder : getFolders(QDir::homePath() +
    // "/.gm-companion/names"))
    //    {
    //        if (!folder.contains("."))
    //        {
    //            m_categories.append(folder);
    //            m_categoryPaths.append(QDir::homePath() +
    // "/.gm-companion/names/" + folder);
    //        }
    //    }

    //    // Addon Names
    //    for (QString path : QStringList({ QDir::homePath() +
    // "/.gm-companion/addons", ":/addons" }))
    //    {
    //        for (QString addon : getFolders(path))
    //        {
    //            if (!addon.contains(".") &&
    // SettingsManager::getInstance()->getIsAddonEnabled(addon) &&
    // (getFolders(path + "/" + addon + "/names").size() > 0))
    //            {
    //                QSettings settings(path + "/" + addon + "/addon.ini",
    // QSettings::IniFormat);
    //                m_categories.append(settings.value("name",
    // addon).toString());
    //                m_categoryPaths.append(path + "/" + addon + "/names");
    //            }
    //        }
    //    }

    emit categoriesChanged();
}

QStringList NameGenerator::categories()
{
    return m_categories;
}

QString NameGenerator::categoryPath(QString category)
{
    int index = m_categories.indexOf(category);

    if (index > -1)
    {
        return m_categoryPaths.at(index);
    }
    else
    {
        qDebug() << "Unknown Category" << category;
        return "";
    }
}

QStringList NameGenerator::categoryNames(QString category)
{
    int index = m_categories.indexOf(category);

    QStringList names;

    //    if (index > -1)
    //    {
    //        QString path = m_categoryPaths.at(index);

    //        for (QString folder : getFolders(path))
    //        {
    //            if (!folder.contains("."))
    //            {
    //                names.append(folder);
    //            }
    //        }
    //    }

    return names;
}

QStringList NameGenerator::maleNames(QString category, QString folder, int count)
{
    return generateNames(category, folder, "male", count);
}

QStringList NameGenerator::femaleNames(QString category, QString folder, int count)
{
    return generateNames(category, folder, "female", count);
}

QStringList NameGenerator::generateNames(QString category, QString folder, QString type, int count)
{
    QStringList names;

    QString path = categoryPath(category) + "/" + folder;

    QString namesPath    = path + "/" + type + ".txt";
    QString surnamesPath = path + "/surname.txt";

    // Forenames
    QStringList forenames;

    if (QFile(namesPath).exists())
    {
        QFile file(namesPath);
        file.open(QIODevice::ReadOnly);
        QString namesAsString = QString::fromLatin1(file.readAll());
        forenames = namesAsString.split(",");
        file.close();
    }

    // Surnames
    QStringList surnames;

    if (QFile(surnamesPath).exists())
    {
        QFile surnamesFile(surnamesPath);
        surnamesFile.open(QIODevice::ReadOnly);
        QString surnamesAsString = QString::fromLatin1(surnamesFile.readAll());
        surnames = surnamesAsString.split(",");
        surnamesFile.close();
    }

    for (int i = 0; i < count; i++)
    {
        QString name = "";

        if (QFile(namesPath).exists())
        {
            name = forenames.at(rand() % forenames.size());

            if (QFile(surnamesPath).exists())
            {
                QString surname = surnames.at(rand() % surnames.size());
                name.append(" " + surname);
            }
        }

        name.replace("\n", "");

        names.append(name);
    }

    return names;
}
