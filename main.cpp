#include "MainWindow.h"
#include "database.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(connectdatabase("flight_book_system"))
    {
        qDebug() << "Connect dataset success!";
    }
    else
    {
        QMessageBox::about(NULL, "提示", "无法连接到数据库");
        return 1;
    }
    MainWindow w;
    w.setWindowTitle("Air ticket booking system");
    w.show();
    return a.exec();
}
