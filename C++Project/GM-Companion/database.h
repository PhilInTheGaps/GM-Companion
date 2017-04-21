#ifndef DATABASE_H
#define DATABASE_H

#include <QStringList>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>

QSqlDatabase initializeDatabase();

QStringList getDatabaseTables(QSqlDatabase);

#endif // DATABASE_H
