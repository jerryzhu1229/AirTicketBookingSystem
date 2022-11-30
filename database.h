#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>
#include <QVariantList>
#include <QMessageBox>
#include <QDebug>

//连接数据库
bool connectdatabase(const QString &dbName)
{
    //测试Qt支持的数据库驱动
    qDebug()<<"Avaliable drivers;";
     QStringList drivers = QSqlDatabase::drivers();
     foreach(QString driver,drivers)
       qDebug()<<driver;

    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("47.108.94.249");                             //连接数据库主机名，这里需要注意（若填的为”127.0.0.1“，出现不能连接，则改为localhost)
    db.setPort(3306);                                        //连接数据库端口号，与设置一致
    db.setDatabaseName(dbName);
    db.setUserName("root");                                  //数据库用户名，与设置一致
    db.setPassword("123456");                                //数据库密码，与设置一致


    if(!db.open())
    {

        qDebug() << db.lastError();
        return false;
    }

    return true;
}
#endif // DATABASE_H
