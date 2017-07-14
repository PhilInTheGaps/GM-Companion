#include "dbmanager.h"

#include <QCoreApplication>
#include <QDebug>

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
