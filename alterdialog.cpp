#include "alterdialog.h"
#include "qradiobutton.h"
#include "ui_alterdialog.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
//这些信息在flightWindow里面初始化
QString old_fr_city;  //改签前出发城市 改签后不变
QString old_to_city; //改签前目的城市 改签后不变
//QString old_seat_type; //改签前舱位类型
QString old_order_code; //改签前订单编号 改签后不变
QString old_flight_code; //改签前航班编号 改签后会变
QString old_seat_code; //改签前座位编号 改签后会变
QString old_passger_tel; //改签前乘客电话 改签后不变
QString old_passger_IDCard; //改签前乘客身份证号 改签后不变
//指向选中的日期的行
int row;
alterdialog::alterdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::alterdialog)
{
    ui->setupUi(this);
    initAlterDialog();
}

alterdialog::~alterdialog()
{
    delete ui;
}

//初始化对话框
void alterdialog::initAlterDialog(){

    row=-1;//初始化为没有选择状态

    //初始化控件样式
    ui->fr_citylab->setStyleSheet("color:#1d4d77");
    ui->to_citylab->setStyleSheet("color:#1d4d77");
    ui->pushButton->setStyleSheet("background-color:rgba(75,120,154,1); color:white");
    ui->pushButton_2->setStyleSheet("background-color:rgba(192,192,192,1); color:white");

    //初始化控件显示信息
    ui->fr_citylab->setText(old_fr_city);
    ui->to_citylab->setText(old_to_city);
    ui->eco_radioBtn->setChecked(true);
    //初始化航班日期信息
    ui->dateTabWidget->horizontalHeader()->setStretchLastSection(true);                    //使行列头自适应宽度，最后一列将会填充空白部分
    ui->dateTabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);     //使行列头自适应宽度，所有列平均分来填充空白部分
    ui->dateTabWidget->horizontalHeader()->setStyleSheet("QHeaderView::section, QTableCornerButton::section{padding: 1px; border:none; border-bottom: 1px solid rgb(75, 120, 154); border-right: 1px solid rgb(75, 120, 154); border-bottom: 1px solid gray; background-color:rgba(75, 120, 154,1); color:#fff;}");
    QSqlTableModel flight_model;
    flight_model.setTable("flight_info");
    flight_model.setFilter(tr("from_city = '%1' and to_city = '%2' and flight_status = '未发机'").arg(old_fr_city).arg(old_to_city));
    flight_model.select();
    int rowcount=flight_model.rowCount();
    for(int i = 0; i < rowcount; i++){
        flight_code = flight_model.data(flight_model.index(i, 0)).toString();
        s_date = flight_model.data(flight_model.index(i, 3)).toString();
        s_time = flight_model.data(flight_model.index(i, 4)).toString();
        f_port = flight_model.data(flight_model.index(i, 8)).toString();
        t_port = flight_model.data(flight_model.index(i, 9)).toString();

        int table_row = ui->dateTabWidget->rowCount();
        ui->dateTabWidget->insertRow(table_row);

        //添加按钮
        QRadioButton *radioBtn = new QRadioButton();
        radioBtn->setObjectName("radioBtn"+ QString::number(i));
        ui->dateTabWidget->setCellWidget(i, 0,radioBtn);
       //设置信号与槽
        connect(radioBtn,SIGNAL(clicked(bool)),this,SLOT(get_row()));

        ui->dateTabWidget->setItem(table_row, 1, new QTableWidgetItem(s_date));
        ui->dateTabWidget->setItem(table_row, 2, new QTableWidgetItem(s_time));
        ui->dateTabWidget->setItem(table_row, 3, new QTableWidgetItem(f_port));
        ui->dateTabWidget->setItem(table_row, 4, new QTableWidgetItem(t_port));
        ui->dateTabWidget->setItem(table_row, 5, new QTableWidgetItem(flight_code));
    }
     ui->dateTabWidget->setColumnHidden(5, true); // 隐藏第5列

}

//将选中那一行的索引存入row
void alterdialog::get_row(){
    row=ui->dateTabWidget->currentRow();//获取当前行
}

//确认改签
void alterdialog::on_pushButton_clicked()
{
     if(is_alter){
         QMessageBox::about(NULL, "提示", "您已改签过一次，不支持再次改签!");
     }
     QString seat_type;//选择的舱位类型
     if(ui->eco_radioBtn->isChecked()){
         seat_type="经济舱";
     }
     if(ui->busi_radiobtn->isChecked()){
         seat_type="商务舱";
     }
     if(ui->best_radiobtn->isChecked()){
         seat_type="头等舱";
     }
     if(row == -1){
         QMessageBox::about(NULL, "提示", "请选择改签日期!");
     }
     else{
     //获得新的航班号
             new_flight_code=ui->dateTabWidget->item(row,5)->text().toUtf8().data();
             //QMessageBox::about(NULL, "行号+航班号", new_flight_code);
             QSqlQueryModel *model = new QSqlQueryModel;
             model->setQuery(QString("select * from ticket_info where flight_code = '%1' and passger_IDCard='0' and seat_type='%2' limit 1;").arg(new_flight_code).arg(seat_type));
             if(model->rowCount()==1){
             new_seat_code=model->index(0,0).data().toString();//新的座位号
             new_order_total=model->index(0,3).data().toString().toInt();//新的票价
             QMessageBox::StandardButton box;
             box = QMessageBox::question(this, "提示", "确定要改签吗?", QMessageBox::Yes|QMessageBox::No);
             if(box==QMessageBox::No)
                   return;
             else{
                 QSqlQuery query1;
                 QSqlQuery query2;
                 QSqlQuery query3;
                 //更新机票信息
                 QString update_seat=QString("update ticket_info set passger_IDCard = '%1' where seat_code = '%2' and flight_code = '%3'")
                         .arg(old_passger_IDCard).arg(new_seat_code).arg(new_flight_code);
                 QString update_order=QString("update order_info set flight_code = '%1',seat_code = '%2',order_total = '%3' where order_code = '%4'")
                         .arg(new_flight_code).arg(new_seat_code).arg(new_order_total).arg(old_order_code);
                 QString update_seat_2=QString("update ticket_info set passger_IDCard = '0' where seat_code = '%2' and flight_code = '%3'")
                         .arg(old_seat_code).arg(old_flight_code);
                 if(query1.exec(update_seat) && query2.exec(update_order)&& query3.exec(update_seat_2)){
                          QMessageBox::about(NULL, "提示", QString("订单%1改签成功,可到订单中心查看订单!").arg(old_order_code));
                          this->close();
              }
             }
    }
    else{
        QMessageBox::about(NULL, "提示", "该次航班暂无!"+seat_type);
    }

       }

}

