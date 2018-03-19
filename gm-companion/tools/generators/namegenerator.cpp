#include "namegenerator.h"
#include "gm-companion/functions.h"

#include <QDebug>
#include <QDir>
#include <QFile>

NameGenerator::NameGenerator(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Name Generator ...";

    updateCategories();
}

void NameGenerator::updateCategories()
{
    qDebug() << "Updating Name Categories ...";

    l_categories.clear();
    l_categoryPaths.clear();

    // Generic Default Names
    l_categories.append("Generic");
    l_categoryPaths.append(":/names/Generic");

    // Custom Names
    for (QString folder : getFolders(QDir::homePath() + "/.gm-companion/names"))
    {
        if (!folder.contains("."))
        {
            l_categories.append(folder);
            l_categoryPaths.append(QDir::homePath() + "/.gm-companion/names/" + folder);
        }
    }

    // Addon Names
    // TODO

    emit categoriesChanged();
}

QStringList NameGenerator::categories()
{
    return l_categories;
}

QString NameGenerator::categoryPath(QString category)
{
    int index = l_categories.indexOf(category);

    if (index > -1)
    {
        return l_categoryPaths.at(index);
    }
    else
    {
        qDebug() << "Unknown Category" << category;
        return "";
    }
}

QStringList NameGenerator::categoryNames(QString category)
{
    int index = l_categories.indexOf(category);

    QStringList names;

    if (index > -1)
    {
        QString path = l_categoryPaths.at(index);

        for (QString folder : getFolders(path))
        {
            if (!folder.contains("."))
            {
                names.append(folder);
            }
        }
    }

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
        QString namesAsString = QString::fromUtf8(file.readAll());
        forenames = namesAsString.split(",");
        file.close();
    }

    // Surnames
    QStringList surnames;

    if (QFile(surnamesPath).exists())
    {
        QFile surnamesFile(surnamesPath);
        surnamesFile.open(QIODevice::ReadOnly);
        QString surnamesAsString = QString::fromUtf8(surnamesFile.readAll());
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
