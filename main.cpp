#include "MainWindow.h"
#include "adminwindow.h"
#include "flightwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include "database.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
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
//    AdminWindow w;
    MainWindow w;
//    FlightWindow w;
    w.setWindowTitle("Air ticket booking system");
    w.show();


//    /*MainWindow w;
//    w.setWindowTitle("航空机票预订系统");
//    w.show();*/
//    //用户主界面，显示、预定航班
//    FlightWindow fly;
//    fly.setWindowTitle("航班预订");
//    fly.show();

    return a.exec();
}
