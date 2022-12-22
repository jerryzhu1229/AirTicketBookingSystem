#include "bookdialog.h"
#include "ui_bookdialog.h"
#include "QFile"
#include "QDebug"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
QString u_name2;//预定者姓名
QString u_tel2;//预定者电话
QString u_IDCard2;//预定者身份证
QString selected_f_code;  //当前航班号
QString selected_f_city;//出发城市
QString selected_t_city;//到达城市
QString selected_s_date;//出发日期
QString selected_s_time;//出发时间

bookDialog::bookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookDialog)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: rgba(255, 255, 255, 1);");
    initBookDialog();//初始化对话框
}

bookDialog::~bookDialog()
{
    delete ui;
}
void bookDialog::initBookDialog(){

    //初始化控件样式
    ui->from_citylab->setStyleSheet("color:#1d4d77");
    ui->to_citylab->setStyleSheet("color:#1d4d77");
    ui->bookpushBtn->setStyleSheet("background-color:rgba(75,120,154,1); color:white");
    ui->cancelBtn->setStyleSheet("background-color:rgba(192,192,192,1); color:white");
    ui->f_codelab->setText(selected_f_code+"次航班");

   //初始化控件显示信息
    ui->from_citylab->setText(selected_f_city);
    ui->to_citylab->setText(selected_t_city);
    ui->f_codelab->setText(selected_f_code);
    ui->start_timelab->setText(selected_s_date+" "+selected_s_time+"出发");

   //查询经济舱相关信息
    QSqlTableModel eco_seat_model;
    eco_seat_model.setTable("ticket_info");
    eco_seat_model.setFilter(tr("flight_code = '%1' and seat_type = '经济舱' and passger_IDCard = '0' ").arg(selected_f_code));
    eco_seat_model.select();
    ecom_num = eco_seat_model.rowCount();//剩余数量
    ecom_price=eco_seat_model.data(eco_seat_model.index(0, 3)).toString().toInt();//单价
    if(ecom_num<=0){
        ui->eco_radioBtn->setEnabled(false);//
    }

    ui->eco_radioBtn->setText("经济舱/"+eco_seat_model.data(eco_seat_model.index(0, 3)).toString()+"元");

    //查询商务舱相关信息
    QSqlTableModel bus_seat_model;
    bus_seat_model.setTable("ticket_info");
    bus_seat_model.setFilter(tr("flight_code = '%1' and seat_type = '商务舱' and passger_IDCard = '0' ").arg(selected_f_code));
    bus_seat_model.select();
    bus_num = bus_seat_model.rowCount();//剩余数量
    bus_price=bus_seat_model.data(bus_seat_model.index(0, 3)).toString().toInt();//单价
    if(bus_num<=0){
        ui->busy_radioBtn->setEnabled(false);//
    }
    ui->busy_radioBtn->setText("商务舱/"+bus_seat_model.data(bus_seat_model.index(0, 3)).toString()+"元");

    //查询头等舱
    QSqlTableModel best_seat_model;
    best_seat_model.setTable("ticket_info");
    best_seat_model.setFilter(tr("flight_code = '%1' and seat_type = '头等舱' and passger_IDCard = '0' ").arg(selected_f_code));
    best_seat_model.select();
    best_num = best_seat_model.rowCount();//剩余数量
    best_price=best_seat_model.data(best_seat_model.index(0, 3)).toString().toInt();//单价
    if(best_num<=0){
        ui->best_radioBtn->setEnabled(false);//
    }
    ui->best_radioBtn->setText("头等舱/"+best_seat_model.data(best_seat_model.index(0, 3)).toString()+"元");

    //初始化乘客信息
    ui->passgTabWidget->horizontalHeader()->setStretchLastSection(true);                    //使行列头自适应宽度，最后一列将会填充空白部分
    ui->passgTabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);     //使行列头自适应宽度，所有列平均分来填充空白部分
    ui->passgTabWidget->horizontalHeader()->setStyleSheet("QHeaderView::section, QTableCornerButton::section{padding: 1px; border:none; border-bottom: 1px solid rgb(75, 120, 154); border-right: 1px solid rgb(75, 120, 154); border-bottom: 1px solid gray; background-color:rgba(75, 120, 154,1); color:#fff;}");
    QSqlTableModel passenger_model;
    passenger_model.setTable("passger_info");
    passenger_model.setFilter(tr("u1_tel = '%1'").arg(u_tel2));
    passenger_model.select();

    int rowcount=passenger_model.rowCount();
    for(int i = 0 ; i < rowcount; i++)
    {
        p_name = passenger_model.data(passenger_model.index(i, 4)).toString();
        p_tel = passenger_model.data(passenger_model.index(i, 5)).toString();
        p_IDCard = passenger_model.data(passenger_model.index(i, 6)).toString();

        int table_row = ui->passgTabWidget->rowCount();
        ui->passgTabWidget->insertRow(table_row);

        QTableWidgetItem *check = new QTableWidgetItem();
        check->setCheckState(Qt::Unchecked);
        ui->passgTabWidget->setItem(table_row,0,check); //插入复选框

        ui->passgTabWidget->setItem(table_row, 1, new QTableWidgetItem(p_name));
        ui->passgTabWidget->setItem(table_row, 2, new QTableWidgetItem(p_tel));
        ui->passgTabWidget->setItem(table_row, 3, new QTableWidgetItem(p_IDCard));

    }

}

//提交预定订单
void bookDialog::on_bookpushBtn_clicked()
{
    QString seat_type;//选择的舱位类型
    if(ui->eco_radioBtn->isChecked()){
        seat_type="经济舱";
    }
    if(ui->busy_radioBtn->isChecked()){
        seat_type="商务舱";
    }
    if(ui->best_radioBtn->isChecked()){
        seat_type="头等舱";
    }

    //订单列表
    QList<int> order_list;
    for(int i = 0; i < ui->passgTabWidget->rowCount(); i++)
    {
        if(ui->passgTabWidget->item(i, 0)->checkState())
        {
           order_list << i;
        }
    }
    if(seat_type.isNull()){
        QMessageBox::about(NULL, "提示", "请选择舱位类型!");
    }
    if(order_list.isEmpty())
    {
        QMessageBox::about(NULL, "提示", "请选择乘客!");
    }

    for(int i = 0; i < order_list.length(); i++)
    {
       //每次查询一张机票
       QSqlQueryModel *model = new QSqlQueryModel;
       model->setQuery(QString("select * from ticket_info where flight_code = '%1' and passger_IDCard='0' and seat_type='%2' limit 1;").arg(selected_f_code).arg(seat_type));
       if(model->rowCount()==1){
           QDateTime local(QDateTime::currentDateTime());
           QString localtime = local.toString("hhmmss");
           QString localdate = local.toString("yyyyMMdd");

           seat_code=model->index(0,0).data().toString();//座位号
           booku_tel=u_tel2;//预定者电话
           passger_tel=ui->passgTabWidget->item(order_list.at(i),2)->text().toUtf8().data();//乘客电话
           passger_IDCard=ui->passgTabWidget->item(order_list.at(i),3)->text().toUtf8().data();//乘客身份证
           order_status="已支付";
           order_total=model->index(0,3).data().toString().toInt();//票价
           create_time = local.toString("yyyy-MM-dd hh:mm:ss");//支付时间
           order_code = (u_tel2 + localdate + localtime).toUtf8().data();//订单编号
           QSqlQuery query1;
           QSqlQuery query2;
           //更新机票信息
           QString update_seat=QString("update ticket_info set passger_IDCard = '%1' where seat_code = '%2' and flight_code = '%3'").arg(passger_IDCard).arg(seat_code).arg(selected_f_code);                                                                  \

           //增加一条订单
           QString add_order = QString("insert into order_info(order_code,flight_code,seat_code,booku_tel,passger_tel,order_total,order_status,create_time)   \
                                        values('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8')")                                                                  \
                                        .arg(order_code).arg(selected_f_code).arg(seat_code).arg(booku_tel).arg(passger_tel)
                                        .arg(order_total).arg(order_status).arg(create_time);
          qDebug()<<"预定者:"+u_name2;
          qDebug()<<selected_f_code+"  "<<seat_code<<" "<<passger_tel<<" "<<passger_IDCard;
          qDebug()<<"order info:";
          qDebug()<<"order_code:"<<order_code <<  " booku_tel:"<<booku_tel << " passger_tel:"<<passger_tel<<" order_total:"<<order_total<<" "<<order_status<<" "<<create_time;
          if(query1.exec(update_seat) && query2.exec(add_order)){
                   QMessageBox::about(NULL, "提示", QString("订单%1下单成功,可到订单中心查看订单!").arg(order_code));
                   ui->passgTabWidget->item(order_list.at(i),0)->setCheckState(Qt::Unchecked);
       }
            else
           {
               QMessageBox::about(NULL, "提示", "下单失败!");
           }

       }else{
           QMessageBox::about(NULL, "提示", "机票不足，请重新预定!");
       }
    }

}

//退出预定
void bookDialog::on_cancelBtn_clicked()
{

}



