#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "MainWindow.h"
#include <QMessageBox>
#include <QDate>
#include <QList>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QSqlTableModel>
#include <qDebug>
#include <QPainter>
#include <QFile>
#include <addflightdialog.h>

//在登录界面初始化这些变量
QString admin_id;


AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    initAdminWindow();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::initAdminWindow()
{

    //设置欢迎信息
    QString welmsg="管理员"+admin_id+",您好,欢迎使用航空机票预订系统》》》";
    ui->welMsgLabel->setText(welmsg);
    ui->welMsgLabel->setStyleSheet("color:#1d4d77");

    //初始化航班日期
    if(ui->query_way_comboBox->currentText()=="航班日期")
    {
        ui->flight_num_label->setVisible(false);
        ui->flight_num_lineEdit->setVisible(false);
    } else {
        ui->flight_date_label->setVisible(false);
        ui->flight_date_dateEdit->setVisible(false);
    }
    //设置日历为当前日期
    QDate local(QDate::currentDate());
    ui->flight_date_dateEdit->setDate(local);

    //设置菜单按钮样式
    set_menuBtnStyle();

    //设置dateedit显示日历，控制日期显示范围
    ui->flight_date_dateEdit->setMinimumDate(QDate::currentDate());
    ui->flight_date_dateEdit->setMaximumDate(QDate::currentDate().addMonths(2));
    ui->flight_date_dateEdit->setCalendarPopup(true);
    //自动完成列头宽度的自动填充，但是最后一列会补满整个剩余列表
    ui->flightTabWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->orderTabWidget_3->horizontalHeader()->setStretchLastSection(true);
    ui->passenageTabWidget->horizontalHeader()->setStretchLastSection(true);
    //均匀拉直表头,完成填充并使列平均分
    ui->flightTabWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->orderTabWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->passenageTabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    show_all_flight_info();
}
//设置菜单按钮样式
void AdminWindow::set_menuBtnStyle(){
    //设置预定按钮的样式(css文件方式)
    QFile f1;
    f1.setFileName(":/qss/css/menubtn.css");
    f1.open(QIODevice::ReadOnly);
    QString menubtnQss = f1.readAll();
    ui->groupBox->setStyleSheet("border:1px solid #1d4d77;");
    ui->flight_Btn->setStyleSheet(menubtnQss);
    ui->order_Btn->setStyleSheet(menubtnQss);
    ui->passenger_Btn->setStyleSheet(menubtnQss);
    ui->logout_pushButton->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->flightQueryBtn_2->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->flight_info_veritfyBtn_3->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->flight_info_deleteBtn_4->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->flight_info_add_button->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->passenger_info_veritfyBtn_4->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->pasnger_info_que_pushButton->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->flight_info_deleteBtn_5->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->order_que_pushButton->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->order_info_veritfyBtn_5->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->order_info_deleteBtn_6->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    f1.close();
}
//给窗体添加背景图片
void AdminWindow::paintEvent(QPaintEvent *event)
{
    QPainter myPainter(this);
    myPainter.setOpacity(0.3);          //背景图片透明度
    myPainter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/img/images/bg3.png"));
}

void AdminWindow::on_query_way_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1=="航班日期")
    {
        ui->flight_num_label->setVisible(false);
        ui->flight_num_lineEdit->setVisible(false);
        ui->flight_date_label->setVisible(true);
        ui->flight_date_dateEdit->setVisible(true);
    } else {
        ui->flight_date_label->setVisible(false);
        ui->flight_date_dateEdit->setVisible(false);
        ui->flight_num_label->setVisible(true);
        ui->flight_num_lineEdit->setVisible(true);
    }
}

void AdminWindow::clear_flight_info_table()
{
    ui->flightTabWidget_2->clearContents();
    ui->flightTabWidget_2->setRowCount(0);
}

void AdminWindow::clear_passenage_info_table()
{
    ui->passenageTabWidget->clearContents();
    ui->passenageTabWidget->setRowCount(0);
}

void AdminWindow::clear_order_info_table()
{
    ui->orderTabWidget_3->clearContents();
    ui->orderTabWidget_3->setRowCount(0);
}
//航班日期查询
void AdminWindow::flight_date_que()
{
    if (ui->query_way_comboBox->currentText()=="航班日期") {
        clear_flight_info_table();
        QString flight_date = ui->flight_date_dateEdit->date().toString("yyyy-MM-dd");

        QSqlTableModel flight_date_model;
        flight_date_model.setTable("flight_info");
        flight_date_model.setFilter(tr("start_date = '%1'").arg(flight_date));
        flight_date_model.select();
        int flight_num = flight_date_model.rowCount();
        if (flight_num>=1) {
            for (int i=0; i<flight_num; ++i) {
                flight_code = flight_date_model.data(flight_date_model.index(i,0)).toString();
                plane_code = flight_date_model.data(flight_date_model.index(i,1)).toString();
                ecomy_price = flight_date_model.data(flight_date_model.index(i,2)).toString();
                start_date = flight_date_model.data(flight_date_model.index(i,3)).toString();
                start_time = flight_date_model.data(flight_date_model.index(i,4)).toString();
                arrive_time = flight_date_model.data(flight_date_model.index(i,5)).toString();
                from_city = flight_date_model.data(flight_date_model.index(i,6)).toString();
                to_city = flight_date_model.data(flight_date_model.index(i,7)).toString();
                from_port = flight_date_model.data(flight_date_model.index(i,8)).toString();
                to_port = flight_date_model.data(flight_date_model.index(i,9)).toString();
                flight_status = flight_date_model.data(flight_date_model.index(i,10)).toString();

                //显示到table widget   第0行是每行标题
                int table_row = ui->flightTabWidget_2->rowCount();
                ui->flightTabWidget_2->insertRow(table_row);

                //插入复选框
                QTableWidgetItem *check = new QTableWidgetItem();
                check->setCheckState(Qt::Unchecked);
                ui->flightTabWidget_2->setItem(table_row, 0, check);

                //插入其它信息
                ui->flightTabWidget_2->setItem(table_row, 1, new QTableWidgetItem(flight_code));
                ui->flightTabWidget_2->setItem(table_row, 2, new QTableWidgetItem(plane_code));
                ui->flightTabWidget_2->setItem(table_row, 3, new QTableWidgetItem(ecomy_price));
                ui->flightTabWidget_2->setItem(table_row, 4, new QTableWidgetItem(start_date));
                ui->flightTabWidget_2->setItem(table_row, 5, new QTableWidgetItem(start_time));
                ui->flightTabWidget_2->setItem(table_row, 6, new QTableWidgetItem(arrive_time));
                ui->flightTabWidget_2->setItem(table_row, 7, new QTableWidgetItem(from_city));
                ui->flightTabWidget_2->setItem(table_row, 8, new QTableWidgetItem(to_city));
                ui->flightTabWidget_2->setItem(table_row, 9, new QTableWidgetItem(from_port));
                ui->flightTabWidget_2->setItem(table_row, 10, new QTableWidgetItem(to_port));
                ui->flightTabWidget_2->setItem(table_row, 11, new QTableWidgetItem(flight_status));

                //读取样式
                QFile f1;
                f1.setFileName(":/qss/css/bookbtn.css");
                f1.open(QIODevice::ReadOnly);
                QString bookbtnQss = f1.readAll();
                //添加修改按钮
                QPushButton *save_alter_Btn = new QPushButton("修改");
                save_alter_Btn->setObjectName("save_alter_Btn"+ QString::number(i));
                save_alter_Btn->setFixedSize(90,30);
                save_alter_Btn->setStyleSheet(bookbtnQss);//添加样式
                ui->flightTabWidget_2->setCellWidget(i, 12,save_alter_Btn);
                //设置信号与槽
                connect(save_alter_Btn,SIGNAL(clicked()),this,SLOT(alter_flight_info()));

                //添加修改按钮
                QPushButton *save_delete_Btn = new QPushButton("删除");
                save_delete_Btn->setObjectName("save_delete_Btn"+ QString::number(i));
                save_delete_Btn->setFixedSize(90,30);
                save_delete_Btn->setStyleSheet(bookbtnQss);//添加样式
                ui->flightTabWidget_2->setCellWidget(i, 13,save_delete_Btn);
                //设置信号与槽
                connect(save_delete_Btn,SIGNAL(clicked()),this,SLOT(delete_flight_info()));

                //设置第一列flight_code内容不可以编辑
                ui->flightTabWidget_2->item(i,1)->setFlags(ui->flightTabWidget_2->item(i,1)->flags() & (~Qt::ItemIsEditable));

            }
        } else {
            QMessageBox::warning(NULL, "提示", "未查询到该日期相关航班信息");
            clear_flight_info_table();
        }
    }
}
//航班号查询
void AdminWindow::flight_num_que()
{
    ui->flightTabWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    if (ui->query_way_comboBox->currentText()=="航班号") {
        QFile f1;
        f1.setFileName(":/qss/css/bookbtn.css");
        f1.open(QIODevice::ReadOnly);
        QString bookbtnQss = f1.readAll();

        QString flight_num = ui->flight_num_lineEdit->text();
        if (flight_num.isEmpty()) {
            QMessageBox::warning(NULL, "提示", "请输入航班号!");
        } else {
            clear_flight_info_table();

            QSqlTableModel flight_num_model;
            flight_num_model.setTable("flight_info");
            flight_num_model.setFilter(tr("flight_code = '%1'").arg(flight_num));
            flight_num_model.select();

            int rowcount = flight_num_model.rowCount();
            if (rowcount == 1) {
                flight_code = flight_num_model.data(flight_num_model.index(0,0)).toString();
                plane_code = flight_num_model.data(flight_num_model.index(0,1)).toString();
                ecomy_price = flight_num_model.data(flight_num_model.index(0,2)).toString();
                start_date = flight_num_model.data(flight_num_model.index(0,3)).toString();
                start_time = flight_num_model.data(flight_num_model.index(0,4)).toString();
                arrive_time = flight_num_model.data(flight_num_model.index(0,5)).toString();
                from_city = flight_num_model.data(flight_num_model.index(0,6)).toString();
                to_city = flight_num_model.data(flight_num_model.index(0,7)).toString();
                from_port = flight_num_model.data(flight_num_model.index(0,8)).toString();
                to_port = flight_num_model.data(flight_num_model.index(0,9)).toString();
                flight_status = flight_num_model.data(flight_num_model.index(0,10)).toString();

                //显示到table widget   第0行是每行标题
                int table_row = ui->flightTabWidget_2->rowCount();
                ui->flightTabWidget_2->insertRow(table_row);

                //插入复选框
                QTableWidgetItem *check = new QTableWidgetItem();
                check->setCheckState(Qt::Unchecked);
                ui->flightTabWidget_2->setItem(table_row, 0, check);

                //插入其它信息
                ui->flightTabWidget_2->setItem(table_row, 1, new QTableWidgetItem(flight_code));
                ui->flightTabWidget_2->setItem(table_row, 2, new QTableWidgetItem(plane_code));
                ui->flightTabWidget_2->setItem(table_row, 3, new QTableWidgetItem(ecomy_price));
                ui->flightTabWidget_2->setItem(table_row, 4, new QTableWidgetItem(start_date));
                ui->flightTabWidget_2->setItem(table_row, 5, new QTableWidgetItem(start_time));
                ui->flightTabWidget_2->setItem(table_row, 6, new QTableWidgetItem(arrive_time));
                ui->flightTabWidget_2->setItem(table_row, 7, new QTableWidgetItem(from_city));
                ui->flightTabWidget_2->setItem(table_row, 8, new QTableWidgetItem(to_city));
                ui->flightTabWidget_2->setItem(table_row, 9, new QTableWidgetItem(from_port));
                ui->flightTabWidget_2->setItem(table_row, 10, new QTableWidgetItem(to_port));
                ui->flightTabWidget_2->setItem(table_row, 11, new QTableWidgetItem(flight_status));
                QFile f1;
                f1.setFileName(":/qss/css/bookbtn.css");
                f1.open(QIODevice::ReadOnly);
                QString bookbtnQss = f1.readAll();
                //添加修改按钮
                QPushButton *save_alter_Btn = new QPushButton("修改");
                save_alter_Btn->setObjectName("save_alter_Btn"+ QString::number(0));
                save_alter_Btn->setFixedSize(90,30);
                save_alter_Btn->setStyleSheet(bookbtnQss);//添加样式
                ui->flightTabWidget_2->setCellWidget(0, 12,save_alter_Btn);
                //设置信号与槽
                connect(save_alter_Btn,SIGNAL(clicked()),this,SLOT(alter_flight_info()));

                //添加修改按钮
                QPushButton *save_delete_Btn = new QPushButton("删除");
                save_delete_Btn->setObjectName("save_delete_Btn"+ QString::number(0));
                save_delete_Btn->setFixedSize(90,30);
                save_delete_Btn->setStyleSheet(bookbtnQss);//添加样式
                ui->flightTabWidget_2->setCellWidget(0, 13,save_delete_Btn);
                //设置信号与槽
                connect(save_delete_Btn,SIGNAL(clicked()),this,SLOT(delete_flight_info()));
                //设置第一列flight_code内容不可以编辑
                ui->flightTabWidget_2->item(0,1)->setFlags(ui->flightTabWidget_2->item(0,1)->flags() & (~Qt::ItemIsEditable));

            } else {
                QMessageBox::warning(NULL, "提示", "未查询到该航班相关信息!");
                ui->flight_num_lineEdit->clear();
                ui->flight_num_lineEdit->setFocus();
                clear_flight_info_table();
            }
        }

    }
}
void AdminWindow::on_flightQueryBtn_2_clicked()
{
    flight_date_que();
    flight_num_que();
}
//航班信息修改 单个
void AdminWindow::alter_flight_info()
{
    int row=ui->flightTabWidget_2->currentRow();//获取当前行
    QString s_flight_code = ui->flightTabWidget_2->item(row,1)->text();
    QString s_plane_code = ui->flightTabWidget_2->item(row,2)->text();
    QString s_ecomy_price = ui->flightTabWidget_2->item(row,3)->text();
    QString s_start_date = ui->flightTabWidget_2->item(row,4)->text();
    QString s_start_time = ui->flightTabWidget_2->item(row,5)->text();
    QString s_arrive_time = ui->flightTabWidget_2->item(row,6)->text();
    QString s_from_city = ui->flightTabWidget_2->item(row,7)->text();
    QString s_to_city = ui->flightTabWidget_2->item(row,8)->text();
    QString s_from_port = ui->flightTabWidget_2->item(row,9)->text();
    QString s_to_port = ui->flightTabWidget_2->item(row,10)->text();
    QString s_flight_status = ui->flightTabWidget_2->item(row,11)->text();
    if(s_plane_code.isEmpty()||s_ecomy_price.isEmpty()||s_start_date.isEmpty()||s_start_time.isEmpty()||
            s_arrive_time.isEmpty()||s_from_city.isEmpty()||s_to_city.isEmpty()||s_from_port.isEmpty()||
            s_to_port.isEmpty()||s_flight_status.isEmpty()){
        QMessageBox::about(NULL, "提示", "请填写完整信息!");
    }else{
        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, "提示", "确定要修改吗?", QMessageBox::Yes|QMessageBox::No);
        if(box==QMessageBox::No)
            return;
        else{
           QString str = QString("update flight_info set plane_code='%1', ecomy_price='%2', start_date='%3', start_time='%4', arrive_time='%5', from_city='%6', to_city='%7', from_port='%8', to_port='%9', flight_status='%10' where flight_code='%11'")
                   .arg(s_plane_code).arg(s_ecomy_price).arg(s_start_date).arg(s_start_time).arg(s_arrive_time).arg(s_from_city).arg(s_to_city).arg(s_from_port).arg(s_to_port).arg(s_flight_status).arg(s_flight_code);

           QSqlQuery query;
           if(query.exec(str))
           {
               QMessageBox::about(NULL, "提示", QString("修改航班%1成功!").arg(ui->flightTabWidget_2->item(row, 1)->text()));
           }
           else
           {
               QMessageBox::warning(NULL, "提示", QString("修改航班%1失败!").arg(ui->flightTabWidget_2->item(row, 1)->text()));
           }
        }
    }
    if(!ui->flight_num_lineEdit->text().isEmpty()) {
        flight_num_que();
    } else {
        show_all_flight_info();
    }
}
//航班信息修改
void AdminWindow::on_flight_info_veritfyBtn_3_clicked()
{
    QList<int> info_list;
    for (int i=0; i<ui->flightTabWidget_2->rowCount(); ++i) {
        if (ui->flightTabWidget_2->item(i,0)->checkState()) {
            info_list << i;
        }
    }

    if (info_list.isEmpty()) {
        QMessageBox::warning(NULL, "提示", "请选择需要修改的航班!");
    } else {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "提示", "确认修改该航班信息吗？", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            for (int i=0; i<info_list.length(); ++i) {
                byte_verifty_flight_code = ui->flightTabWidget_2->item(info_list.at(i),1)->text().toUtf8();
                verifty_flight_code = byte_verifty_flight_code.data();

                QSqlTableModel flight_info_model;
                flight_info_model.setTable("flight_info");
                flight_info_model.setFilter(tr("flight_code = '%1'").arg(ui->flightTabWidget_2->item(info_list.at(i),1)->text()));
                flight_info_model.select();

                int rowCount = flight_info_model.rowCount();
                if (rowCount ==1 ) {
                    byte_verifty_plane_code = ui->flightTabWidget_2->item(info_list.at(i),2)->text().toUtf8();
                    byte_verifty_ecomy_price = ui->flightTabWidget_2->item(info_list.at(i),3)->text().toUtf8();
                    byte_verifty_start_date = ui->flightTabWidget_2->item(info_list.at(i),4)->text().toUtf8();
                    byte_verifty_start_time = ui->flightTabWidget_2->item(info_list.at(i),5)->text().toUtf8();
                    byte_verifty_arrive_time = ui->flightTabWidget_2->item(info_list.at(i),6)->text().toUtf8();
                    byte_verifty_from_city = ui->flightTabWidget_2->item(info_list.at(i),7)->text().toUtf8();
                    byte_verifty_to_city = ui->flightTabWidget_2->item(info_list.at(i),8)->text().toUtf8();
                    byte_verifty_from_port = ui->flightTabWidget_2->item(info_list.at(i),9)->text().toUtf8();
                    byte_verifty_to_port = ui->flightTabWidget_2->item(info_list.at(i),10)->text().toUtf8();
                    byte_verifty_flight_status = ui->flightTabWidget_2->item(info_list.at(i),11)->text().toUtf8();

                    verifty_plane_code = byte_verifty_plane_code.data();
                    verifty_ecomy_price = byte_verifty_ecomy_price.data();
                    verifty_start_date = byte_verifty_start_date.data();
                    verifty_start_time = byte_verifty_start_time.data();
                    verifty_arrive_time = byte_verifty_arrive_time.data();
                    verifty_from_city = byte_verifty_from_city.data();
                    verifty_to_city = byte_verifty_to_city.data();
                    verifty_from_port = byte_verifty_from_port.data();
                    verifty_to_port = byte_verifty_to_port.data();
                    verifty_flight_status = byte_verifty_flight_status.data();

                    QString str = QString("update flight_info set plane_code='%1', ecomy_price='%2', start_date='%3', start_time='%4', arrive_time='%5', from_city='%6', to_city='%7', from_port='%8', to_port='%9', flight_status='%10' where flight_code='%11'")
                            .arg(verifty_plane_code).arg(verifty_ecomy_price).arg(verifty_start_date).arg(verifty_start_time).arg(verifty_arrive_time).arg(verifty_from_city).arg(verifty_to_city).arg(verifty_from_port).arg(verifty_to_port).arg(verifty_flight_status).arg(verifty_flight_code);

                    QSqlQuery query;
                    if(query.exec(str))
                    {
                        QMessageBox::about(NULL, "提示", QString("修改航班%1成功!").arg(ui->flightTabWidget_2->item(info_list.at(i), 1)->text()));
                        ui->flightTabWidget_2->item(info_list.at(i), 0)->setCheckState(Qt::Unchecked);
                    }
                    else
                    {
                        QMessageBox::warning(NULL, "提示", QString("修改航班%1失败!").arg(ui->flightTabWidget_2->item(info_list.at(i), 1)->text()));
                    }
                }
            }
        }
        if (reply == QMessageBox::No)
        {
            QMessageBox::about(NULL, "提示", "您已取消航班修改!");
        }
    }
    if(!ui->flight_num_lineEdit->text().isEmpty()) {
        flight_num_que();
    } else {
        show_all_flight_info();
    }
}
//航班信息删除 单个
void AdminWindow::delete_flight_info()
{
    int row=ui->flightTabWidget_2->currentRow();//获取当前行
    QString s_flight_code = ui->flightTabWidget_2->item(row,1)->text();
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this, "提示", "确定要修改吗?", QMessageBox::Yes|QMessageBox::No);
    if(box==QMessageBox::No)
        return;
    else{
        QSqlTableModel flight_info_model;
        flight_info_model.setTable("flight_info");
        flight_info_model.setFilter(tr("flight_code = '%1'").arg(s_flight_code));
        flight_info_model.select();

        int rowCount = flight_info_model.rowCount();
        if (rowCount ==1 ) {
            //删除航班表
            QString str1 = QString("delete from flight_info where flight_code='%1'").arg(s_flight_code);
            QSqlQuery query1;
            //删除ticket表
            QString str2 = QString("delete from ticket_info where flight_code='%1'").arg(s_flight_code);
            QSqlQuery query2;
            //删除order表
            QString str3 = QString("delete from order_info where flight_code='%1'").arg(s_flight_code);
            QSqlQuery query3;
            if(query1.exec(str1) && query2.exec(str2) && query3.exec(str3))
            {
                QMessageBox::about(NULL, "提示", QString("删除航班%1成功!").arg(ui->flightTabWidget_2->item(row, 1)->text()));
            }
            else
            {
                QMessageBox::warning(NULL, "提示", QString("删除航班%1失败!").arg(ui->flightTabWidget_2->item(row, 1)->text()));
            }
        }
    }
    if(!ui->flight_num_lineEdit->text().isEmpty()) {
        flight_num_que();
    } else {
        show_all_flight_info();
    }
}
//航班信息删除
void AdminWindow::on_flight_info_deleteBtn_4_clicked()
{
    QList<int> info_list;
    for (int i=0; i<ui->flightTabWidget_2->rowCount(); ++i) {
        if (ui->flightTabWidget_2->item(i,0)->checkState()) {
            info_list << i;
        }
    }

    if (info_list.isEmpty()) {
        QMessageBox::warning(NULL, "提示", "请选择需要删除的航班!");
    } else {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "提示", "确认删除该航班信息吗？", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            for (int i=0; i<info_list.length(); ++i) {
                byte_verifty_flight_code = ui->flightTabWidget_2->item(info_list.at(i),1)->text().toUtf8();
                verifty_flight_code = byte_verifty_flight_code.data();

                QSqlTableModel flight_info_model;
                flight_info_model.setTable("flight_info");
                flight_info_model.setFilter(tr("flight_code = '%1'").arg(ui->flightTabWidget_2->item(info_list.at(i),1)->text()));
                flight_info_model.select();

                int rowCount = flight_info_model.rowCount();
                if (rowCount ==1 ) {
                    //删除航班表
                    QString str1 = QString("delete from flight_info where flight_code='%1'").arg(verifty_flight_code);
                    QSqlQuery query1;
                    //删除ticket表
                    QString str2 = QString("delete from ticket_info where flight_code='%1'").arg(verifty_flight_code);
                    QSqlQuery query2;
                    //删除order表
                    QString str3 = QString("delete from order_info where flight_code='%1'").arg(verifty_flight_code);
                    QSqlQuery query3;
                    if(query1.exec(str1) && query2.exec(str2) && query3.exec(str3))
                    {

                        QMessageBox::about(NULL, "提示", QString("删除航班%1成功!").arg(ui->flightTabWidget_2->item(info_list.at(i), 1)->text()));
                        ui->flightTabWidget_2->item(info_list.at(i), 0)->setCheckState(Qt::Unchecked);

                    }
                    else
                    {
                        QMessageBox::warning(NULL, "提示", QString("删除航班%1失败!").arg(ui->flightTabWidget_2->item(info_list.at(i), 1)->text()));
                    }
                }
            }
        }
        if (reply == QMessageBox::No)
        {
            QMessageBox::about(NULL, "提示", "您已取消航班删除!");
        }
    }
    if(!ui->flight_num_lineEdit->text().isEmpty()) {
        flight_num_que();
    } else {
        show_all_flight_info();
    }
}

// 显示所有航班信息
void AdminWindow::show_all_flight_info()
{
    ui->flightTabWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    clear_flight_info_table();

    QSqlTableModel flight_num_model;
    flight_num_model.setTable("flight_info");
    flight_num_model.select();

    int row = flight_num_model.rowCount();
    for (int i=0; i<row; ++i) {
        QString flight_code = flight_num_model.data(flight_num_model.index(i,0)).toString();
        plane_code = flight_num_model.data(flight_num_model.index(i,1)).toString();
        ecomy_price = flight_num_model.data(flight_num_model.index(i,2)).toString();
        start_date = flight_num_model.data(flight_num_model.index(i,3)).toString();
        start_time = flight_num_model.data(flight_num_model.index(i,4)).toString();
        arrive_time = flight_num_model.data(flight_num_model.index(i,5)).toString();
        from_city = flight_num_model.data(flight_num_model.index(i,6)).toString();
        to_city = flight_num_model.data(flight_num_model.index(i,7)).toString();
        from_port = flight_num_model.data(flight_num_model.index(i,8)).toString();
        to_port = flight_num_model.data(flight_num_model.index(i,9)).toString();
        flight_status = flight_num_model.data(flight_num_model.index(i,10)).toString();
        //显示到table widget   第0行是每行标题
        int table_row = ui->flightTabWidget_2->rowCount();
        ui->flightTabWidget_2->insertRow(table_row);
        //插入复选框
        QTableWidgetItem *check = new QTableWidgetItem();
        check->setCheckState(Qt::Unchecked);
        ui->flightTabWidget_2->setItem(table_row, 0, check);
        //插入其它信息
        ui->flightTabWidget_2->setItem(table_row, 1, new QTableWidgetItem(flight_code));
        ui->flightTabWidget_2->setItem(table_row, 2, new QTableWidgetItem(plane_code));
        ui->flightTabWidget_2->setItem(table_row, 3, new QTableWidgetItem(ecomy_price));
        ui->flightTabWidget_2->setItem(table_row, 4, new QTableWidgetItem(start_date));
        ui->flightTabWidget_2->setItem(table_row, 5, new QTableWidgetItem(start_time));
        ui->flightTabWidget_2->setItem(table_row, 6, new QTableWidgetItem(arrive_time));
        ui->flightTabWidget_2->setItem(table_row, 7, new QTableWidgetItem(from_city));
        ui->flightTabWidget_2->setItem(table_row, 8, new QTableWidgetItem(to_city));
        ui->flightTabWidget_2->setItem(table_row, 9, new QTableWidgetItem(from_port));
        ui->flightTabWidget_2->setItem(table_row, 10, new QTableWidgetItem(to_port));
        ui->flightTabWidget_2->setItem(table_row, 11, new QTableWidgetItem(flight_status));

        QFile f1;
        f1.setFileName(":/qss/css/bookbtn.css");
        f1.open(QIODevice::ReadOnly);
        QString bookbtnQss = f1.readAll();
        //添加修改按钮
        QPushButton *save_alter_Btn = new QPushButton("修改");
        save_alter_Btn->setObjectName("save_alter_Btn"+ QString::number(i));
        save_alter_Btn->setFixedSize(90,30);
        save_alter_Btn->setStyleSheet(bookbtnQss);//添加样式
        ui->flightTabWidget_2->setCellWidget(i, 12,save_alter_Btn);
        //设置信号与槽
        connect(save_alter_Btn,SIGNAL(clicked()),this,SLOT(alter_flight_info()));
        //添加修改按钮
        QPushButton *save_delete_Btn = new QPushButton("删除");
        save_delete_Btn->setObjectName("save_delete_Btn"+ QString::number(i));
        save_delete_Btn->setFixedSize(90,30);
        save_delete_Btn->setStyleSheet(bookbtnQss);//添加样式
        ui->flightTabWidget_2->setCellWidget(i, 13,save_delete_Btn);
        //设置信号与槽
        connect(save_delete_Btn,SIGNAL(clicked()),this,SLOT(delete_flight_info()));
        //设置第一列flight_code内容不可以编辑
        ui->flightTabWidget_2->item(0,1)->setFlags(ui->flightTabWidget_2->item(0,1)->flags() & (~Qt::ItemIsEditable));

    }
}
// 显示所有旅客信息
void AdminWindow::show_all_passenage_info()
{
    ui->passenageTabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    clear_passenage_info_table();

    QSqlTableModel user_num_model;
    user_num_model.setTable("user_info");
    user_num_model.select();

    int row = user_num_model.rowCount();
    for (int i=0; i<row; ++i) {
        user_name = user_num_model.data(user_num_model.index(i,0)).toString();
        user_code = user_num_model.data(user_num_model.index(i,1)).toString();
        user_tel = user_num_model.data(user_num_model.index(i,2)).toString();
        user_pwd = user_num_model.data(user_num_model.index(i,3)).toString();

        //显示到table widget   第0行是每行标题
        int table_row = ui->passenageTabWidget->rowCount();
        ui->passenageTabWidget->insertRow(table_row);

        //插入复选框
        QTableWidgetItem *check = new QTableWidgetItem();
        check->setCheckState(Qt::Unchecked);
        ui->passenageTabWidget->setItem(table_row, 0, check);

        //插入其它信息
        ui->passenageTabWidget->setItem(table_row, 1, new QTableWidgetItem(user_name));
        ui->passenageTabWidget->setItem(table_row, 2, new QTableWidgetItem(user_code));
        ui->passenageTabWidget->setItem(table_row, 3, new QTableWidgetItem(user_tel));
        ui->passenageTabWidget->setItem(table_row, 4, new QTableWidgetItem(user_pwd));
        QFile f1;
        f1.setFileName(":/qss/css/bookbtn.css");
        f1.open(QIODevice::ReadOnly);
        QString bookbtnQss = f1.readAll();
        //添加修改按钮
        QPushButton *save_alter_Btn = new QPushButton("修改");
        save_alter_Btn->setObjectName("save_alter_Btn"+ QString::number(i));
        save_alter_Btn->setFixedSize(90,30);
        save_alter_Btn->setStyleSheet(bookbtnQss);//添加样式
        ui->passenageTabWidget->setCellWidget(i, 5,save_alter_Btn);
        //设置信号与槽
        connect(save_alter_Btn,SIGNAL(clicked()),this,SLOT(alter_passger_info()));

        //添加删除按钮
        QPushButton *save_delete_Btn = new QPushButton("删除");
        save_delete_Btn->setObjectName("save_delete_Btn"+ QString::number(i));
        save_delete_Btn->setFixedSize(90,30);
        save_delete_Btn->setStyleSheet(bookbtnQss);//添加样式
        ui->passenageTabWidget->setCellWidget(i, 6,save_delete_Btn);
        //设置信号与槽
        connect(save_delete_Btn,SIGNAL(clicked()),this,SLOT(delete_passger_info()));
        //设置第三列内容不可以编辑
        ui->passenageTabWidget->item(0,3)->setFlags(ui->passenageTabWidget->item(0,3)->flags() & (~Qt::ItemIsEditable));
    }
}

void AdminWindow::show_all_order_info()
{
    ui->orderTabWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    clear_order_info_table();
    QSqlTableModel order_num_model;
    order_num_model.setTable("order_info");
    order_num_model.select();

    int rowcount = order_num_model.rowCount();
    for (int i=0; i<rowcount; ++i) {

        order_code = order_num_model.data(order_num_model.index(i,0)).toString();
        QString flight_code_ = order_num_model.data(order_num_model.index(i,1)).toString();
        seat_code = order_num_model.data(order_num_model.index(i,2)).toString();
        booku_tel = order_num_model.data(order_num_model.index(i,3)).toString();
        passger_tel = order_num_model.data(order_num_model.index(i,4)).toString();
        order_total = order_num_model.data(order_num_model.index(i,5)).toString();
        order_status = order_num_model.data(order_num_model.index(i,6)).toString();
        create_time = order_num_model.data(order_num_model.index(i,7)).toString();

        //显示到table widget   第0行是每行标题
        int table_row = ui->orderTabWidget_3->rowCount();
        ui->orderTabWidget_3->insertRow(table_row);

        //插入复选框
        QTableWidgetItem *check = new QTableWidgetItem();
        check->setCheckState(Qt::Unchecked);
        ui->orderTabWidget_3->setItem(table_row, 0, check);

        //插入其它信息
        ui->orderTabWidget_3->setItem(table_row, 1, new QTableWidgetItem(order_code));
        ui->orderTabWidget_3->setItem(table_row, 2, new QTableWidgetItem(flight_code_));
        ui->orderTabWidget_3->setItem(table_row, 3, new QTableWidgetItem(seat_code));
        ui->orderTabWidget_3->setItem(table_row, 4, new QTableWidgetItem(booku_tel));
        ui->orderTabWidget_3->setItem(table_row, 5, new QTableWidgetItem(passger_tel));
        ui->orderTabWidget_3->setItem(table_row, 6, new QTableWidgetItem(order_total));
        ui->orderTabWidget_3->setItem(table_row, 7, new QTableWidgetItem(order_status));
        ui->orderTabWidget_3->setItem(table_row, 8, new QTableWidgetItem(create_time));

        QFile f1;
        f1.setFileName(":/qss/css/bookbtn.css");
        f1.open(QIODevice::ReadOnly);
        QString bookbtnQss = f1.readAll();
        //添加修改按钮
        QPushButton *save_alter_Btn = new QPushButton("修改");
        save_alter_Btn->setObjectName("save_alter_Btn"+ QString::number(i));
        save_alter_Btn->setFixedSize(90,30);
        save_alter_Btn->setStyleSheet(bookbtnQss);//添加样式
        ui->orderTabWidget_3->setCellWidget(i, 9,save_alter_Btn);
        //设置信号与槽
        connect(save_alter_Btn,SIGNAL(clicked()),this,SLOT(alter_order_info()));

        //添加删除按钮
        QPushButton *save_delete_Btn = new QPushButton("删除");
        save_delete_Btn->setObjectName("save_delete_Btn"+ QString::number(i));
        save_delete_Btn->setFixedSize(90,30);
        save_delete_Btn->setStyleSheet(bookbtnQss);//添加样式
        ui->orderTabWidget_3->setCellWidget(i, 10,save_delete_Btn);
        //设置信号与槽
        connect(save_delete_Btn,SIGNAL(clicked()),this,SLOT(delete_order_info()));
        //设置第一列内容不可以编辑
        ui->orderTabWidget_3->item(0,1)->setFlags(ui->orderTabWidget_3->item(0,1)->flags() & (~Qt::ItemIsEditable));
    }
}
//显示所有航班信息
void AdminWindow::on_flight_Btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->flight_page_2);
    show_all_flight_info();
}

//显示所有乘客信息
void AdminWindow::on_passenger_Btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->passenage_page_2);
    show_all_passenage_info();
}

//显示所有订单信息
void AdminWindow::on_order_Btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->order_page_2);
    show_all_order_info();
}

//用户信息查询
void AdminWindow::on_pasnger_info_que_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->passenageTabWidget);
    passenage_tel_que();
}

void AdminWindow::passenage_tel_que()
{
    ui->passenageTabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QString u_tel = ui->tel_num_lineEdit->text();
    if (u_tel.isEmpty()) {
        QMessageBox::warning(NULL, "提示", "请输入手机号!");
    } else {
        clear_passenage_info_table();
        QSqlTableModel user_num_model;
        user_num_model.setTable("user_info");
        user_num_model.setFilter(tr("u_tel = '%1'").arg(u_tel));
        user_num_model.select();

        int rowcount = user_num_model.rowCount();
        if (rowcount == 1) {
            user_name = user_num_model.data(user_num_model.index(0,0)).toString();
            user_code = user_num_model.data(user_num_model.index(0,1)).toString();
            user_tel = user_num_model.data(user_num_model.index(0,2)).toString();
            user_pwd = user_num_model.data(user_num_model.index(0,3)).toString();

            //显示到table widget   第0行是每行标题
            int table_row = ui->passenageTabWidget->rowCount();
            ui->passenageTabWidget->insertRow(table_row);

            //插入复选框
            QTableWidgetItem *check = new QTableWidgetItem();
            check->setCheckState(Qt::Unchecked);
            ui->passenageTabWidget->setItem(table_row, 0, check);

            //插入其它信息
            ui->passenageTabWidget->setItem(table_row, 1, new QTableWidgetItem(user_name));
            ui->passenageTabWidget->setItem(table_row, 2, new QTableWidgetItem(user_code));
            ui->passenageTabWidget->setItem(table_row, 3, new QTableWidgetItem(user_tel));
            ui->passenageTabWidget->setItem(table_row, 4, new QTableWidgetItem(user_pwd));

            QFile f1;
            f1.setFileName(":/qss/css/bookbtn.css");
            f1.open(QIODevice::ReadOnly);
            QString bookbtnQss = f1.readAll();
            //添加修改按钮
            QPushButton *save_alter_Btn = new QPushButton("修改");
            save_alter_Btn->setObjectName("save_alter_Btn"+ QString::number(0));
            save_alter_Btn->setFixedSize(90,30);
            save_alter_Btn->setStyleSheet(bookbtnQss);//添加样式
            ui->passenageTabWidget->setCellWidget(0, 5,save_alter_Btn);
            //设置信号与槽
            connect(save_alter_Btn,SIGNAL(clicked()),this,SLOT(alter_passger_info()));

            //添加删除按钮
            QPushButton *save_delete_Btn = new QPushButton("删除");
            save_delete_Btn->setObjectName("save_delete_Btn"+ QString::number(0));
            save_delete_Btn->setFixedSize(90,30);
            save_delete_Btn->setStyleSheet(bookbtnQss);//添加样式
            ui->passenageTabWidget->setCellWidget(0, 6,save_delete_Btn);
            //设置信号与槽
            connect(save_delete_Btn,SIGNAL(clicked()),this,SLOT(delete_passger_info()));

            //设置第三列内容不可以编辑
            ui->passenageTabWidget->item(0,3)->setFlags(ui->passenageTabWidget->item(0,3)->flags() & (~Qt::ItemIsEditable));

        } else {
            QMessageBox::warning(NULL, "提示", "未查询到该用户相关信息!");
            ui->tel_num_lineEdit->clear();
            ui->tel_num_lineEdit->setFocus();
            clear_passenage_info_table();
        }
    }
}
//旅客信息修改 单
void AdminWindow::alter_passger_info()
{
    int row=ui->passenageTabWidget->currentRow();//获取当前行
    QString s_user_name = ui->passenageTabWidget->item(row,1)->text();
    QString s_user_code = ui->passenageTabWidget->item(row,2)->text();
    QString s_user_tel = ui->passenageTabWidget->item(row,3)->text();
    QString s_user_pwd = ui->passenageTabWidget->item(row,4)->text();

    if(s_user_name.isEmpty()||s_user_code.isEmpty()||s_user_tel.isEmpty()||s_user_pwd.isEmpty()){
        QMessageBox::about(NULL, "提示", "请填写完整信息!");
    }else{
        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, "提示", "确定要修改吗?", QMessageBox::Yes|QMessageBox::No);
        if(box==QMessageBox::No)
            return;
        else{
            QString str_password = ui->passenageTabWidget->item(row,4)->text();
            QByteArray byte_password = str_password.toLatin1();
            QByteArray byte_password_md5 = QCryptographicHash::hash(byte_password,QCryptographicHash::Md5);
            QString strPwdMd5 = byte_password_md5.toHex();
            QByteArray b_password_md5 = strPwdMd5.toLatin1();
            char * c_password_md5 = b_password_md5.data();
            QString str = QString("update user_info set u_name='%1', u_IDCard='%2', u_pwd='%3' where u_tel='%4'")
                    .arg(s_user_name).arg(s_user_code).arg(c_password_md5).arg(s_user_tel);

            QSqlQuery query;
            if(query.exec(str))
            {
                QMessageBox::about(NULL, "提示", QString("修改用户%1信息成功!").arg(ui->passenageTabWidget->item(row, 1)->text()));
                ui->passenageTabWidget->item(row, 0)->setCheckState(Qt::Unchecked);

            }
            else
            {
                QMessageBox::warning(NULL, "提示", QString("修改用户%1失败!").arg(ui->passenageTabWidget->item(row, 1)->text()));
            }
        }
    }
    if (!ui->tel_num_lineEdit->text().isEmpty()) {
        passenage_tel_que();

    } else {
        show_all_passenage_info();
    }
}
//旅客信息修改
void AdminWindow::on_passenger_info_veritfyBtn_4_clicked()
{
    QList<int> info_list;
    for (int i=0; i<ui->passenageTabWidget->rowCount(); ++i) {
        if (ui->passenageTabWidget->item(i,0)->checkState()) {
            info_list << i;
        }
    }

    if (info_list.isEmpty()) {
        QMessageBox::warning(NULL, "提示", "请选择需要修改的用户!");
    } else {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "提示", "确认修改该用户信息吗？", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            for (int i=0; i<info_list.length(); ++i) {
                byte_verifty_user_tel = ui->passenageTabWidget->item(info_list.at(i),3)->text().toUtf8();
                verifty_user_tel = byte_verifty_user_tel.data();

                QSqlTableModel user_info_model;
                user_info_model.setTable("user_info");
                user_info_model.setFilter(tr("u_tel = '%1'").arg(ui->passenageTabWidget->item(info_list.at(i),3)->text()));
                user_info_model.select();

                int rowCount = user_info_model.rowCount();
                if (rowCount ==1 ) {
                    byte_verifty_user_name = ui->passenageTabWidget->item(info_list.at(i),1)->text().toUtf8();
                    byte_verifty_user_code = ui->passenageTabWidget->item(info_list.at(i),2)->text().toUtf8();
                    byte_verifty_user_pwd = ui->passenageTabWidget->item(info_list.at(i),4)->text().toUtf8();

                    verifty_user_name = byte_verifty_user_name.data();
                    verifty_user_code = byte_verifty_user_code.data();
                    verifty_user_pwd = byte_verifty_user_pwd.data();

                    QString str_password = ui->passenageTabWidget->item(info_list.at(i),4)->text();
                    QByteArray byte_password = str_password.toLatin1();
                    QByteArray byte_password_md5 = QCryptographicHash::hash(byte_password,QCryptographicHash::Md5);
                    QString strPwdMd5 = byte_password_md5.toHex();
                    QByteArray b_password_md5 = strPwdMd5.toLatin1();
                    char * c_password_md5 = b_password_md5.data();
                    QString str = QString("update user_info set u_name='%1', u_IDCard='%2', u_pwd='%3' where u_tel='%4'")
                            .arg(verifty_user_name).arg(verifty_user_code).arg(c_password_md5).arg(verifty_user_tel);


                    QSqlQuery query;
                    if(query.exec(str))
                    {
                        QMessageBox::about(NULL, "提示", QString("修改用户%1信息成功!").arg(ui->passenageTabWidget->item(info_list.at(i), 1)->text()));
                        ui->passenageTabWidget->item(info_list.at(i), 0)->setCheckState(Qt::Unchecked);

                    }
                    else
                    {
                        QMessageBox::warning(NULL, "提示", QString("修改用户%1失败!").arg(ui->passenageTabWidget->item(info_list.at(i), 1)->text()));
                    }
                }
            }
        }
        if (reply == QMessageBox::No)
        {
            QMessageBox::about(NULL, "提示", "您已取消旅客信息修改!");
        }
    }
    if (!ui->tel_num_lineEdit->text().isEmpty()) {
        passenage_tel_que();

    } else {
        show_all_passenage_info();
    }
}
//用户信息删除 单
void AdminWindow::delete_passger_info()
{
    int row=ui->passenageTabWidget->currentRow();//获取当前行
    QString s_user_name = ui->passenageTabWidget->item(row,1)->text();
    QString s_user_code = ui->passenageTabWidget->item(row,2)->text();
    QString s_user_tel = ui->passenageTabWidget->item(row,3)->text();
    QString s_user_pwd = ui->passenageTabWidget->item(row,4)->text();

    if(s_user_name.isEmpty()||s_user_code.isEmpty()||s_user_tel.isEmpty()||s_user_pwd.isEmpty()){
        QMessageBox::about(NULL, "提示", "请填写完整信息!");
    }else{
        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, "提示", "确定要修改吗?", QMessageBox::Yes|QMessageBox::No);
        if(box==QMessageBox::No)
            return;
        else{

            QByteArray byte_verifty_u_IDCard = ui->passenageTabWidget->item(row,2)->text().toUtf8();
            char *verifty_u_IDCard = byte_verifty_u_IDCard.data();
            QSqlTableModel flight_info_model;
            flight_info_model.setTable("user_info");
            flight_info_model.setFilter(tr("u_tel = '%1'").arg(ui->passenageTabWidget->item(row,3)->text()));
            flight_info_model.select();

            int rowCount = flight_info_model.rowCount();
            if (rowCount ==1 ) {
                //删除user
                QString str1 = QString("delete from user_info where u_tel='%1'").arg(s_user_tel);
                QSqlQuery query1;

                //删除passger
                QString str2 = QString("delete from passger_info where u1_tel='%1'").arg(s_user_tel);
                QSqlQuery query2;

                //删除order
                QString str3 = QString("delete from order_info where passger_tel='%1'").arg(s_user_tel);
                QSqlQuery query3;

                //修改ticket表
                QString str4 = QString("update ticket_info set passger_IDCard='0' where passger_IDCard='%1'").arg(verifty_u_IDCard);
                QSqlQuery query4;
                if(query1.exec(str1) && query2.exec(str2))
                {
                    QMessageBox::about(NULL, "提示", QString("删除用户%1信息成功!").arg(ui->passenageTabWidget->item(row, 1)->text()));
                    ui->passenageTabWidget->item(row, 0)->setCheckState(Qt::Unchecked);
                    query3.exec(str3);
                    query4.exec(str4);
                    ui->stackedWidget->setCurrentWidget(ui->passenage_page_2);

                }
                else
                {
                    QMessageBox::warning(NULL, "提示", QString("删除用户%1失败!").arg(ui->passenageTabWidget->item(row, 1)->text()));
                }
            }
        }
    }
    if (!ui->tel_num_lineEdit->text().isEmpty()) {
        passenage_tel_que();

    } else {
        show_all_passenage_info();
    }
}
//用户信息删除
void AdminWindow::on_flight_info_deleteBtn_5_clicked()
{
    QList<int> info_list;
    for (int i=0; i<ui->passenageTabWidget->rowCount(); ++i) {
        if (ui->passenageTabWidget->item(i,0)->checkState()) {
            info_list << i;
        }
    }

    if (info_list.isEmpty()) {
        QMessageBox::warning(NULL, "提示", "请选择需要删除的用户!");
    } else {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "提示", "确认删除该用户信息吗？", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            for (int i=0; i<info_list.length(); ++i) {
                byte_verifty_user_tel = ui->passenageTabWidget->item(info_list.at(i),3)->text().toUtf8();
                verifty_user_tel = byte_verifty_user_tel.data();

                QByteArray byte_verifty_u_IDCard = ui->passenageTabWidget->item(info_list.at(i),2)->text().toUtf8();
                char *verifty_u_IDCard = byte_verifty_u_IDCard.data();
                QSqlTableModel flight_info_model;
                flight_info_model.setTable("user_info");
                flight_info_model.setFilter(tr("u_tel = '%1'").arg(ui->passenageTabWidget->item(info_list.at(i),3)->text()));
                flight_info_model.select();

                int rowCount = flight_info_model.rowCount();
                if (rowCount ==1 ) {
                    //删除user
                    QString str1 = QString("delete from user_info where u_tel='%1'").arg(verifty_user_tel);
                    QSqlQuery query1;

                    //删除passger
                    QString str2 = QString("delete from passger_info where u1_tel='%1'").arg(verifty_user_tel);
                    QSqlQuery query2;

                    //删除order
                    QString str3 = QString("delete from order_info where passger_tel='%1'").arg(verifty_user_tel);
                    QSqlQuery query3;

                    //修改ticket表
                    QString str4 = QString("update ticket_info set passger_IDCard='0' where passger_IDCard='%1'").arg(verifty_u_IDCard);
                    QSqlQuery query4;
                    if(query1.exec(str1) && query2.exec(str2))
                    {
                        QMessageBox::about(NULL, "提示", QString("删除用户%1信息成功!").arg(ui->passenageTabWidget->item(info_list.at(i), 1)->text()));
                        ui->passenageTabWidget->item(info_list.at(i), 0)->setCheckState(Qt::Unchecked);
                        query3.exec(str3);
                        query4.exec(str4);
                        ui->stackedWidget->setCurrentWidget(ui->passenage_page_2);

                    }
                    else
                    {
                        QMessageBox::warning(NULL, "提示", QString("删除用户%1失败!").arg(ui->passenageTabWidget->item(info_list.at(i), 1)->text()));
                    }
                }
            }
        }
        if (reply == QMessageBox::No)
        {
            QMessageBox::about(NULL, "提示", "您已取消旅客信息修改!");
        }
    }
    if (!ui->tel_num_lineEdit->text().isEmpty()) {
        passenage_tel_que();

    }  else {
        show_all_passenage_info();

    }
}

//订单查询
void AdminWindow::order_que()
{
    clear_order_info_table();
    ui->orderTabWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QString order_code_ = ui->order_num_lineEdit->text();
    if (order_code_.isEmpty()) {
        QMessageBox::warning(NULL, "提示", "请输入订单号!");
    } else {

        QSqlTableModel order_num_model;
        order_num_model.setTable("order_info");
        order_num_model.setFilter(tr("order_code = '%1'").arg(order_code_));
        order_num_model.select();

        int rowcount = order_num_model.rowCount();
        if (rowcount == 1) {

            order_code = order_num_model.data(order_num_model.index(0,0)).toString();
            flight_code = order_num_model.data(order_num_model.index(0,1)).toString();
            seat_code = order_num_model.data(order_num_model.index(0,2)).toString();
            booku_tel = order_num_model.data(order_num_model.index(0,3)).toString();
            passger_tel = order_num_model.data(order_num_model.index(0,4)).toString();
            order_total = order_num_model.data(order_num_model.index(0,5)).toString();
            order_status = order_num_model.data(order_num_model.index(0,6)).toString();
            create_time = order_num_model.data(order_num_model.index(0,7)).toString();

            //显示到table widget   第0行是每行标题
            int table_row = ui->orderTabWidget_3->rowCount();
            ui->orderTabWidget_3->insertRow(table_row);

            //插入复选框
            QTableWidgetItem *check = new QTableWidgetItem();
            check->setCheckState(Qt::Unchecked);
            ui->orderTabWidget_3->setItem(table_row, 0, check);

            //插入其它信息
            ui->orderTabWidget_3->setItem(table_row, 1, new QTableWidgetItem(order_code));
            ui->orderTabWidget_3->setItem(table_row, 2, new QTableWidgetItem(flight_code));
            ui->orderTabWidget_3->setItem(table_row, 3, new QTableWidgetItem(seat_code));
            ui->orderTabWidget_3->setItem(table_row, 4, new QTableWidgetItem(booku_tel));
            ui->orderTabWidget_3->setItem(table_row, 5, new QTableWidgetItem(passger_tel));
            ui->orderTabWidget_3->setItem(table_row, 6, new QTableWidgetItem(order_total));
            ui->orderTabWidget_3->setItem(table_row, 7, new QTableWidgetItem(order_status));
            ui->orderTabWidget_3->setItem(table_row, 8, new QTableWidgetItem(create_time));

            QFile f1;
            f1.setFileName(":/qss/css/bookbtn.css");
            f1.open(QIODevice::ReadOnly);
            QString bookbtnQss = f1.readAll();
            //添加修改按钮
            QPushButton *save_alter_Btn = new QPushButton("修改");
            save_alter_Btn->setObjectName("save_alter_Btn"+ QString::number(0));
            save_alter_Btn->setFixedSize(90,30);
            save_alter_Btn->setStyleSheet(bookbtnQss);//添加样式
            ui->orderTabWidget_3->setCellWidget(0, 9,save_alter_Btn);
            //设置信号与槽
            connect(save_alter_Btn,SIGNAL(clicked()),this,SLOT(alter_order_info()));

            //添加删除按钮
            QPushButton *save_delete_Btn = new QPushButton("删除");
            save_delete_Btn->setObjectName("save_delete_Btn"+ QString::number(0));
            save_delete_Btn->setFixedSize(90,30);
            save_delete_Btn->setStyleSheet(bookbtnQss);//添加样式
            ui->orderTabWidget_3->setCellWidget(0, 10,save_delete_Btn);
            //设置信号与槽
            connect(save_delete_Btn,SIGNAL(clicked()),this,SLOT(delete_order_info()));

            //设置第一列内容不可以编辑
            ui->orderTabWidget_3->item(0,1)->setFlags(ui->orderTabWidget_3->item(0,1)->flags() & (~Qt::ItemIsEditable));

        } else {
            QMessageBox::warning(NULL, "提示", "未查询到该订单相关信息!");
            ui->order_num_lineEdit->clear();
            ui->order_num_lineEdit->setFocus();
            clear_order_info_table();
        }
    }
}

void AdminWindow::on_order_que_pushButton_clicked()
{
    order_que();
}

//订单修改 单
void AdminWindow::alter_order_info()
{
    int row=ui->orderTabWidget_3->currentRow();//获取当前行
    QString s_order_code = ui->orderTabWidget_3->item(row,1)->text();
    QString s_flight_code = ui->orderTabWidget_3->item(row,2)->text();
    QString s_seat_code = ui->orderTabWidget_3->item(row,3)->text();
    QString s_booku_tel = ui->orderTabWidget_3->item(row,4)->text();
    QString s_passger_tel = ui->orderTabWidget_3->item(row,5)->text();
    QString s_order_total = ui->orderTabWidget_3->item(row,6)->text();
    QString s_order_status = ui->orderTabWidget_3->item(row,7)->text();
    QString s_create_time = ui->orderTabWidget_3->item(row,8)->text();

    if(s_order_code.isEmpty()||s_flight_code.isEmpty()||s_seat_code.isEmpty()||s_booku_tel.isEmpty()||
            s_passger_tel.isEmpty()||s_order_total.isEmpty()||s_order_status.isEmpty()||s_create_time.isEmpty()){
        QMessageBox::about(NULL, "提示", "请填写完整信息!");
    }else{
        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, "提示", "确定要修改吗?", QMessageBox::Yes|QMessageBox::No);
        if(box==QMessageBox::No)
            return;
        else{
            QString str = QString("update order_info set flight_code='%1', seat_code='%2', booku_tel='%3', passger_tel='%4', order_total='%5', order_status='%6', create_time='%7' where order_code='%8'")
                    .arg(s_flight_code,s_seat_code,s_booku_tel,s_passger_tel,s_order_total,s_order_status,s_create_time,s_order_code);
//                    QString str = QString("update order_info set flight_code='%1', seat_code='%2'where order_code='%3'")
//                            .arg(verifty_flight_code,verifty_seat_code,verifty_order_code);

            QSqlQuery query;
            qDebug() << query.exec(str);
            query.exec(str);
            if(query.exec(str))
            {
                QMessageBox::about(NULL, "提示", QString("修改订单信息成功!"));
            }
            else
            {
                QMessageBox::warning(NULL, "提示", QString("修改订单失败!"));
            }
        }
    }
    if (!ui->order_num_lineEdit->text().isEmpty()) {
        order_que();
    } else {
        show_all_order_info();
    }
}
//订单修改
void AdminWindow::on_order_info_veritfyBtn_5_clicked()
{
    QList<int> info_list;
    for (int i=0; i<ui->orderTabWidget_3->rowCount(); ++i) {
        if (ui->orderTabWidget_3->item(i,0)->checkState()) {
            info_list << i;
        }
    }

    if (info_list.isEmpty()) {
        QMessageBox::warning(NULL, "提示", "请选择需要修改的订单!");
    } else {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "提示", "确认修改该订单信息吗？", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            for (int i=0; i<info_list.length(); ++i) {
                byte_verifty_order_code = ui->orderTabWidget_3->item(info_list.at(i),1)->text().toUtf8();
                verifty_order_code = byte_verifty_order_code.data();

                QSqlTableModel order_info_model;
                order_info_model.setTable("order_info");
                order_info_model.setFilter(tr("order_code = '%1'").arg(ui->orderTabWidget_3->item(info_list.at(i),1)->text()));
                order_info_model.select();

                int rowCount = order_info_model.rowCount();
                if (rowCount ==1 ) {
                    byte_verifty_flight_code = ui->orderTabWidget_3->item(info_list.at(i),2)->text().toUtf8();
                    byte_verifty_seat_code = ui->orderTabWidget_3->item(info_list.at(i),3)->text().toUtf8();
                    byte_verifty_booku_tel = ui->orderTabWidget_3->item(info_list.at(i),4)->text().toUtf8();
                    byte_verifty_passger_tel = ui->orderTabWidget_3->item(info_list.at(i),5)->text().toUtf8();
                    QByteArray byte_verifty_order_total = ui->orderTabWidget_3->item(info_list.at(i),6)->text().toUtf8();
                    QByteArray byte_verifty_order_status = ui->orderTabWidget_3->item(info_list.at(i),7)->text().toUtf8();
                    byte_verifty_create_time = ui->orderTabWidget_3->item(info_list.at(i),8)->text().toUtf8();

                    verifty_flight_code = byte_verifty_flight_code.data();
                    verifty_seat_code = byte_verifty_seat_code.data();
                    verifty_booku_tel = byte_verifty_booku_tel.data();
                    verifty_passger_tel = byte_verifty_passger_tel.data();
                    char* verifty_order_total = byte_verifty_order_total.data();
                    qDebug() << verifty_order_total;
                    char* verifty_order_status = byte_verifty_order_status.data();
                    qDebug() << verifty_order_status;
                    verifty_create_time = byte_verifty_create_time.data();


                    QString str = QString("update order_info set flight_code='%1', seat_code='%2', booku_tel='%3', passger_tel='%4', order_total='%5', order_status='%6', create_time='%7' where order_code='%8'")
                            .arg(verifty_flight_code,verifty_seat_code,verifty_booku_tel,verifty_passger_tel,verifty_order_total,verifty_order_status,verifty_create_time,verifty_order_code);
//                    QString str = QString("update order_info set flight_code='%1', seat_code='%2'where order_code='%3'")
//                            .arg(verifty_flight_code,verifty_seat_code,verifty_order_code);

                    QSqlQuery query;
                    qDebug() << query.exec(str);
                    query.exec(str);
                    if(query.exec(str))
                    {
                        QMessageBox::about(NULL, "提示", QString("修改订单信息成功!"));
                        ui->orderTabWidget_3->item(info_list.at(i), 0)->setCheckState(Qt::Unchecked);

                    }
                    else
                    {
                        QMessageBox::warning(NULL, "提示", QString("修改订单失败!"));
                    }
                }
            }
        }
        if (reply == QMessageBox::No)
        {
            QMessageBox::about(NULL, "提示", "您已取消订单信息修改!");
        }
    }
    if (!ui->order_num_lineEdit->text().isEmpty()) {
        order_que();
    } else {
        show_all_order_info();
    }
}
//订单删除 单
void AdminWindow::delete_order_info()
{
    int row=ui->passenageTabWidget->currentRow();//获取当前行
    QString s_order_code = ui->orderTabWidget_3->item(row,1)->text();
    QString s_flight_code = ui->orderTabWidget_3->item(row,2)->text();
    QString s_seat_code = ui->orderTabWidget_3->item(row,3)->text();
    QString s_booku_tel = ui->orderTabWidget_3->item(row,4)->text();
    QString s_passger_tel = ui->orderTabWidget_3->item(row,5)->text();
    QString s_order_total = ui->orderTabWidget_3->item(row,6)->text();
    QString s_order_status = ui->orderTabWidget_3->item(row,7)->text();
    QString s_create_time = ui->orderTabWidget_3->item(row,8)->text();

    if(s_order_code.isEmpty()||s_flight_code.isEmpty()||s_seat_code.isEmpty()||s_booku_tel.isEmpty()||
            s_passger_tel.isEmpty()||s_order_total.isEmpty()||s_order_status.isEmpty()||s_create_time.isEmpty()){
        QMessageBox::about(NULL, "提示", "请填写完整信息!");
    }else{
        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, "提示", "确定要修改吗?", QMessageBox::Yes|QMessageBox::No);
        if(box==QMessageBox::No)
            return;
        else{

            QSqlTableModel order_info_model;
            order_info_model.setTable("order_info");
            order_info_model.setFilter(tr("order_code = '%1'").arg(ui->orderTabWidget_3->item(row,1)->text()));
            order_info_model.select();

            int rowCount = order_info_model.rowCount();
            if (rowCount ==1 ) {
                //删除order表
                QString str1 = QString("delete from order_info where order_code='%1'")
                        .arg(s_order_code);
                QSqlQuery query1;
                qDebug()<<str1;

                //修改ticket表
                QString str2 = QString("update ticket_info set passger_IDCard='0' where flight_code='%1' and seat_code='%2'").arg(s_flight_code,s_seat_code);
                QSqlQuery query2;
                qDebug()<<str2;
//                    if(query2.exec(str2))
                if(query1.exec(str1) && query2.exec(str2))
                {
                    QMessageBox::about(NULL, "提示", QString("删除订单%1信息成功!").arg(ui->orderTabWidget_3->item(row, 1)->text()));
                    ui->stackedWidget->setCurrentWidget(ui->order_page_2);
                }
                else
                {
                    QMessageBox::warning(NULL, "提示", QString("删除订单%1失败!").arg(ui->orderTabWidget_3->item(row, 1)->text()));
                }
            }
        }
    }
    if (!ui->order_num_lineEdit->text().isEmpty()) {
        order_que();
    } else {
        show_all_order_info();
    }
}
//订单删除
void AdminWindow::on_order_info_deleteBtn_6_clicked()
{
    QList<int> info_list;
    for (int i=0; i<ui->orderTabWidget_3->rowCount(); ++i) {
        if (ui->orderTabWidget_3->item(i,0)->checkState()) {
            info_list << i;
        }
    }

    if (info_list.isEmpty()) {
        QMessageBox::warning(NULL, "提示", "请选择需要删除的订单!");
    } else {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "提示", "确认删除该订单信息吗？", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            for (int i=0; i<info_list.length(); ++i) {
                byte_verifty_order_code = ui->orderTabWidget_3->item(info_list.at(i),1)->text().toUtf8();
                verifty_order_code = byte_verifty_order_code.data();
                QByteArray byte_verifty_flight_code = ui->orderTabWidget_3->item(info_list.at(i),2)->text().toUtf8();
                char * verifty_flight_code = byte_verifty_flight_code.data();
                QByteArray byte_verifty_seat_code = ui->orderTabWidget_3->item(info_list.at(i),3)->text().toUtf8();
                char * verifty_seat_code = byte_verifty_seat_code.data();
                QSqlTableModel order_info_model;
                order_info_model.setTable("order_info");
                order_info_model.setFilter(tr("order_code = '%1'").arg(ui->orderTabWidget_3->item(info_list.at(i),1)->text()));
                order_info_model.select();

                int rowCount = order_info_model.rowCount();
                if (rowCount ==1 ) {
                    //删除order表
                    QString str1 = QString("delete from order_info where order_code='%1'")
                            .arg(verifty_order_code);
                    QSqlQuery query1;
                    qDebug()<<str1;

                    //修改ticket表
                    QString str2 = QString("update ticket_info set passger_IDCard='0' where flight_code='%1' and seat_code='%2'").arg(verifty_flight_code,verifty_seat_code);
                    QSqlQuery query2;
                    qDebug()<<str2;
//                    if(query2.exec(str2))
                    if(query1.exec(str1) && query2.exec(str2))
                    {
                        QMessageBox::about(NULL, "提示", QString("删除订单%1信息成功!").arg(ui->orderTabWidget_3->item(info_list.at(i), 1)->text()));
                        ui->orderTabWidget_3->item(info_list.at(i), 0)->setCheckState(Qt::Unchecked);
                        ui->stackedWidget->setCurrentWidget(ui->order_page_2);

                    }
                    else
                    {
                        QMessageBox::warning(NULL, "提示", QString("删除订单%1失败!").arg(ui->orderTabWidget_3->item(info_list.at(i), 1)->text()));
                    }
                }
            }
        }
        if (reply == QMessageBox::No)
        {
            QMessageBox::about(NULL, "提示", "您已取消订单信息修改!");
        }
    }
    if (!ui->order_num_lineEdit->text().isEmpty()) {
        order_que();
    } else {
        show_all_order_info();
    }
}


void AdminWindow::on_logout_pushButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "提示", "确认退出管理员吗?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        this->close();
        MainWindow * mw = new MainWindow();
        mw->setWindowTitle("航空机票预订系统");
        mw->show();
    }
}


void AdminWindow::on_flight_info_add_button_clicked()
{
    //弹出对话框
    addflightdialog add_flight;
    add_flight.setWindowTitle("填写航班信息");
    add_flight.setModal(false);
    add_flight.exec();
}

