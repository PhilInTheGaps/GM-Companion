#include "database.h"
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlDriver>

QSqlDatabase initializeDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Datenbanken/CharakterDatenbank.sql");
    bool db_ok = db.open();
    return db;
}

QStringList getDatabaseTables(QSqlDatabase db){
    QStringList tables = db.tables();

    return tables;
}
