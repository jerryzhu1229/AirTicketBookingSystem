#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QDebug>


bool connectdatabase(const QString &dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("47.108.94.249");
    db.setPort(3306);
    db.setDatabaseName(dbName);
    db.setUserName("root");
    db.setPassword("123456");

    if(!db.open())
    {
        qDebug()<<"can't connect database!";
        return false;
    }
    return true;
}

#endif // DATABASE_H
