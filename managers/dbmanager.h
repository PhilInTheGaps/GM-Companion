#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>

class DbManager
{
public:
    DbManager(const QString& path);

    void addTable(QString name);
    void removeTable(QString name);
    QStringList getTables();


private:
    QSqlDatabase m_db;

//    QSqlQuery query(m_db);
};

#endif // DBMANAGER_H
