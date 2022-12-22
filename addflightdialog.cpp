#include "addflightdialog.h"
#include "ui_addflightdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <qDebug>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QFile>
#include <QSqlTableModel>
addflightdialog::addflightdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addflightdialog)
{
    ui->setupUi(this);

    connect(ui->from_city_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(OnSelectChanged_fromcity()));
    connect(ui->to_city_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(OnSelectChanged_tocity()));
    initaddflightdialog();
}

addflightdialog::~addflightdialog()
{
    delete ui;
}

void addflightdialog::OnSelectChanged_fromcity(){
    if(ui->from_city_comboBox->currentIndex() == 0){ //判断选择的为第0项
        ui->from_port_comboBox->clear();                 //清除ComboBox中的内容
        ui->from_port_comboBox->addItem("首都");        //添加下拉列表项
        ui->from_port_comboBox->addItem("大兴");
    } else if(ui->from_city_comboBox->currentIndex() == 1){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("虹桥");
        ui->from_port_comboBox->addItem("浦东");
    } else if(ui->from_city_comboBox->currentIndex() == 2){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("白云");
    } else if(ui->from_city_comboBox->currentIndex() == 3){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("宝安");
    } else if(ui->from_city_comboBox->currentIndex() == 4){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("双流");
        ui->from_port_comboBox->addItem("天府");
    } else if(ui->from_city_comboBox->currentIndex() == 5){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("萧山");
    } else if(ui->from_city_comboBox->currentIndex() == 6){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("咸阳");
    } else if(ui->from_city_comboBox->currentIndex() == 7){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("江北");
    } else if(ui->from_city_comboBox->currentIndex() == 8){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("胶东");
    } else if(ui->from_city_comboBox->currentIndex() == 9){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("禄口");
    } else if(ui->from_city_comboBox->currentIndex() == 10){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("高崎");
    } else if(ui->from_city_comboBox->currentIndex() == 11){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("长水");
    } else if(ui->from_city_comboBox->currentIndex() == 12){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("周水子");
    } else if(ui->from_city_comboBox->currentIndex() == 13){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("滨海");
    } else if(ui->from_city_comboBox->currentIndex() == 14){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("新郑");
    } else if(ui->from_city_comboBox->currentIndex() == 15){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("凤凰");
    } else if(ui->from_city_comboBox->currentIndex() == 16){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("遥墙");
    } else if(ui->from_city_comboBox->currentIndex() == 17){
        ui->from_port_comboBox->clear();
        ui->from_port_comboBox->addItem("长乐");
    }
}
void addflightdialog::OnSelectChanged_tocity(){
    if(ui->to_city_comboBox->currentIndex() == 0){ //判断选择的为第0项
        ui->to_port_comboBox->clear();                 //清除ComboBox中的内容
        ui->to_port_comboBox->addItem("首都");        //添加下拉列表项
        ui->to_port_comboBox->addItem("大兴");
    } else if(ui->to_city_comboBox->currentIndex() == 1){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("虹桥");
        ui->to_port_comboBox->addItem("浦东");
    } else if(ui->to_city_comboBox->currentIndex() == 2){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("白云");
    } else if(ui->to_city_comboBox->currentIndex() == 3){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("宝安");
    } else if(ui->to_city_comboBox->currentIndex() == 4){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("双流");
        ui->to_port_comboBox->addItem("天府");
    } else if(ui->to_city_comboBox->currentIndex() == 5){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("萧山");
    } else if(ui->to_city_comboBox->currentIndex() == 6){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("咸阳");
    } else if(ui->to_city_comboBox->currentIndex() == 7){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("江北");
    } else if(ui->to_city_comboBox->currentIndex() == 8){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("胶东");
    } else if(ui->to_city_comboBox->currentIndex() == 9){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("禄口");
    } else if(ui->to_city_comboBox->currentIndex() == 10){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("高崎");
    } else if(ui->to_city_comboBox->currentIndex() == 11){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("长水");
    } else if(ui->to_city_comboBox->currentIndex() == 12){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("周水子");
    } else if(ui->to_city_comboBox->currentIndex() == 13){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("滨海");
    } else if(ui->to_city_comboBox->currentIndex() == 14){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("新郑");
    } else if(ui->to_city_comboBox->currentIndex() == 15){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("凤凰");
    } else if(ui->to_city_comboBox->currentIndex() == 16){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("遥墙");
    } else if(ui->to_city_comboBox->currentIndex() == 17){
        ui->to_port_comboBox->clear();
        ui->to_port_comboBox->addItem("长乐");
    }
}


void addflightdialog::initaddflightdialog(){
    //初始化机型
    initPlane();

    //初始化城市
    initCity();

    //设置时间为当前日期
    QDate local_date(QDate::currentDate());
    ui->start_date_dateEdit->setDate(local_date);
    QTime local_time(QTime::currentTime());
    ui->start_time_timeEdit->setTime(local_time);
    QDateTime local_date_time(QDateTime::currentDateTime());
    ui->arrive_dateTimeEdit->setDateTime(local_date_time);

    //设置dateedit显示日历，控制日期显示范围
    ui->start_date_dateEdit->setMinimumDate(QDate::currentDate());
    ui->start_date_dateEdit->setMaximumDate(QDate::currentDate().addMonths(2));
    ui->start_date_dateEdit->setCalendarPopup(true);

    //设置预定按钮的样式(css文件方式)
    QFile f1;
    f1.setFileName(":/qss/css/menubtn.css");
    f1.open(QIODevice::ReadOnly);
    QString menubtnQss = f1.readAll();
     ui->pushButton->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    f1.close();
}
//初始化机型
void addflightdialog::initPlane(){
    ui->comboBox->setStyleSheet("text-align:center; color:#1d4d77;");
    QSqlTableModel plane_model;
    plane_model.setTable("plane_info");
    plane_model.select();
    int plane_num = plane_model.rowCount();
    for (int i=0; i<plane_num; ++i) {
        QString plane_code = plane_model.data(plane_model.index(i,0)).toString();
        ui->comboBox->insertItem(i, plane_code);
    }
}

//初始化城市
void addflightdialog::initCity(){

    ui->from_city_comboBox->setStyleSheet("text-align:center; color:#1d4d77;");
    ui->to_city_comboBox->setStyleSheet("text-align:center; color:#1d4d77;");

    /*出发城市*/
    ui->from_city_comboBox->insertItem(0,  "北京");
    ui->from_city_comboBox->insertItem(1,  "上海");
    ui->from_city_comboBox->insertItem(2,  "广州");
    ui->from_city_comboBox->insertItem(3,  "深圳");
    ui->from_city_comboBox->insertItem(4,  "成都");
    ui->from_city_comboBox->insertItem(5,  "杭州");
    ui->from_city_comboBox->insertItem(6,  "西安");
    ui->from_city_comboBox->insertItem(7,  "重庆");
    ui->from_city_comboBox->insertItem(8,  "青岛");
    ui->from_city_comboBox->insertItem(9,  "南京");
    ui->from_city_comboBox->insertItem(10, "厦门");
    ui->from_city_comboBox->insertItem(11, "昆明");
    ui->from_city_comboBox->insertItem(12, "大连");
    ui->from_city_comboBox->insertItem(13, "天津");
    ui->from_city_comboBox->insertItem(14, "郑州");
    ui->from_city_comboBox->insertItem(15, "三亚");
    ui->from_city_comboBox->insertItem(16, "济南");
    ui->from_city_comboBox->insertItem(17, "福州");

    /*目的城市*/
    ui->to_city_comboBox->insertItem(0,  "北京");
    ui->to_city_comboBox->insertItem(1,  "上海");
    ui->to_city_comboBox->insertItem(2,  "广州");
    ui->to_city_comboBox->insertItem(3,  "深圳");
    ui->to_city_comboBox->insertItem(4,  "成都");
    ui->to_city_comboBox->insertItem(5,  "杭州");
    ui->to_city_comboBox->insertItem(6,  "西安");
    ui->to_city_comboBox->insertItem(7,  "重庆");
    ui->to_city_comboBox->insertItem(8,  "青岛");
    ui->to_city_comboBox->insertItem(9,  "南京");
    ui->to_city_comboBox->insertItem(10, "厦门");
    ui->to_city_comboBox->insertItem(11, "昆明");
    ui->to_city_comboBox->insertItem(12, "大连");
    ui->to_city_comboBox->insertItem(13, "天津");
    ui->to_city_comboBox->insertItem(14, "郑州");
    ui->to_city_comboBox->insertItem(15, "三亚");
    ui->to_city_comboBox->insertItem(16, "济南");
    ui->to_city_comboBox->insertItem(17, "福州");
}
//添加航班
void addflightdialog::on_pushButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "提示", "确认添加该航班信息吗？", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {

        QByteArray byte_verifty_flight_code = ui->flight_num_lineEdit->text().toUtf8();
        QByteArray byte_verifty_plane_code = ui->comboBox->currentText().toUtf8();
        QByteArray byte_verifty_ecomy_price = ui->eco_money_lineEdit->text().toUtf8();
        QByteArray byte_verifty_start_date = ui->start_date_dateEdit->date().toString("yyyy-MM-dd").toLatin1();
        QByteArray byte_verifty_start_time = ui->start_time_timeEdit->time().toString("HH:mm:ss").toLatin1();
        QByteArray byte_verifty_arrive_time = ui->arrive_dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss").toLatin1();
        QByteArray byte_verifty_from_city = ui->from_city_comboBox->currentText().toUtf8();
        QByteArray byte_verifty_to_city = ui->to_city_comboBox->currentText().toUtf8();
        QByteArray byte_verifty_from_port = ui->from_port_comboBox->currentText().toUtf8();
        QByteArray byte_verifty_to_port = ui->to_port_comboBox->currentText().toUtf8();
        QByteArray byte_verifty_flight_status = QString("未发机").toUtf8();

        char * verifty_flight_code = byte_verifty_flight_code.data();
        char * verifty_plane_code = byte_verifty_plane_code.data();
        char * verifty_ecomy_price = byte_verifty_ecomy_price.data();
        char * verifty_start_date = byte_verifty_start_date.data();
        char * verifty_start_time = byte_verifty_start_time.data();
        char * verifty_arrive_time = byte_verifty_arrive_time.data();
        char * verifty_from_city = byte_verifty_from_city.data();
        char * verifty_to_city = byte_verifty_to_city.data();
        char * verifty_from_port = byte_verifty_from_port.data();
        char * verifty_to_port = byte_verifty_to_port.data();
        char * verifty_flight_status = byte_verifty_flight_status.data();
        //添加航班
        QString str1 = QString("insert into flight_info(flight_code, plane_code, ecomy_price, start_date, start_time, arrive_time, from_city, to_city, from_port, to_port, flight_status) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11')")
                .arg(verifty_flight_code).arg(verifty_plane_code).arg(verifty_ecomy_price).arg(verifty_start_date).arg(verifty_start_time).arg(verifty_arrive_time).arg(verifty_from_city).arg(verifty_to_city).arg(verifty_from_port).arg(verifty_to_port).arg(verifty_flight_status);
        qDebug() << str1;
        QSqlQuery query1;
        //添加座位

        if(query1.exec(str1))
        {
            QMessageBox::about(NULL, "提示", QString("增加航班%1成功!").arg(ui->flight_num_lineEdit->text()));
            //添加座位
            int eco_num = ui->eco_num_lineEdit->text().toInt();
            for (int i=1;i<=eco_num;++i){
                QString seat_code = 'E' + QString::number(i);
//                qDebug() << seat_code;
                QString str2 = QString("insert into ticket_info(seat_code, flight_code, seat_type,ticket_price,passger_IDCard) values('%1','%2','%3','%4','0')")
                        .arg(seat_code.toUtf8().data()).arg(verifty_flight_code).arg(QString("经济舱").toUtf8().data()).arg(ui->eco_money_lineEdit->text().toUtf8().data());
                QSqlQuery query;
//                qDebug() << str2;
                if(query.exec(str2)) {
                    qDebug()<<QString("添加%1座位成功").arg(seat_code);
                }
            }
            int busi_num = ui->busi_num_lineEdit->text().toInt();
            for (int i=1;i<=busi_num;++i){
                QString seat_code = 'B' + QString::number(i);
//                qDebug() << seat_code;
                QString str2 = QString("insert into ticket_info(seat_code, flight_code, seat_type,ticket_price,passger_IDCard) values('%1','%2','%3','%4','0')")
                        .arg(seat_code.toUtf8().data()).arg(verifty_flight_code).arg(QString("商务舱").toUtf8().data()).arg(ui->busi_money_lineEdit->text().toUtf8().data());
                QSqlQuery query;
//                qDebug() << str2;
                if(query.exec(str2)) {
                    qDebug()<<QString("添加%1座位成功").arg(seat_code);
                }
            }
            int fastclas_num = ui->firstclass_num_lineEdit->text().toInt();
            for (int i=1;i<=fastclas_num;++i){
                QString seat_code = 'F' + QString::number(i);
//                qDebug() << seat_code;
                QString str2 = QString("insert into ticket_info(seat_code, flight_code, seat_type,ticket_price,passger_IDCard) values('%1','%2','%3','%4','0')")
                        .arg(seat_code.toUtf8().data()).arg(verifty_flight_code).arg(QString("头等舱").toUtf8().data()).arg(ui->firstclass_money_lineEdit->text().toUtf8().data());
                QSqlQuery query;
//                qDebug() << str2;
                if(query.exec(str2)) {
                    qDebug()<<QString("添加%1座位成功").arg(seat_code);
                }
            }

            ui->flight_num_lineEdit->clear();
            ui->eco_money_lineEdit->clear();
//            ui->start_date_lineEdit->clear();
//            ui->start_time_lineEdit->clear();
//            ui->arrive_time_lineEdit->clear();
            ui->eco_money_lineEdit->clear();
            ui->eco_num_lineEdit->clear();
            ui->busi_money_lineEdit->clear();
            ui->busi_num_lineEdit->clear();
            ui->firstclass_money_lineEdit->clear();
            ui->firstclass_num_lineEdit->clear();
        }
        else
        {
            QMessageBox::warning(NULL, "提示", QString("增加航班%1失败!").arg(ui->flight_num_lineEdit->text()));
        }

    }
    if (reply == QMessageBox::No)
    {
        QMessageBox::about(NULL, "提示", "您已取消航班添加!");
    }
}

