#include "dbmanager.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSqlRecord>

DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
       m_db.setDatabaseName(path);

       if (!m_db.open())
       {
          qDebug() << QCoreApplication::translate("DbManager", "Error: connection with database failed");
       }
       else
       {
          qDebug() << QCoreApplication::translate("DbManager", "Database: connection ok");
       }
}

void DbManager::addTable(QString name)
{
    QSqlQuery query(m_db);
    query.prepare("CREATE TABLE :name (id INTEGER);");

    query.bindValue(":name", name);

    query.exec();

    qDebug() << "Added table " << name;
}

void DbManager::removeTable(QString name)
{

}

QStringList DbManager::getTables()
{
    QStringList tables;

    QSqlQuery query(m_db);
    query.prepare("SELECT name FROM sqlite_master WHERE type='table';");

    int idName = query.record().indexOf("name");
    while (query.next())
    {
       QString name = query.value(idName).toString();
       qDebug() << name;
    }

    return tables;
}
