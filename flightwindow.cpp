#include "flightwindow.h"
#include "ui_flightwindow.h"
#include "bookdialog.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
//在登录界面初始化这些变量
QString u_name;
QString u_tel;
QString u_IDCard;

//这是bookDialog要用到的信息
extern QString u_name2;//预定者姓名
extern QString u_tel2;//预定者电话
extern QString u_IDCard2;//预定者身份证
extern QString selected_f_code;  //当前航班号
extern QString selected_f_city;//出发城市
extern QString selected_t_city;//到达城市
extern QString selected_s_date;//出发日期
extern QString selected_s_time;//出发时间

FlightWindow::FlightWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlightWindow)
{
    ui->setupUi(this);
    initUser_info();  //初始化用户信息
    initFlightWindow(); //初始化主界面
}

FlightWindow::~FlightWindow()
{
    delete ui;
}
//初始化用户信息
void FlightWindow::initUser_info(){
   qDebug()<<u_name;
   qDebug()<<u_tel;
   qDebug()<<u_IDCard;
}

//初始化主界面
void FlightWindow::initFlightWindow(){

    //设置欢迎信息
    QString welmsg=u_name+",您好,欢迎登录航空机票预订系统》》》";
    ui->welMsgLabel->setText(welmsg);
    ui->welMsgLabel->setStyleSheet("color:#1d4d77");

    //初始化城市信息
    initCity();

    //设置菜单按钮样式
    set_menuBtnStyle();

    //设置dateedit显示日历，控制日期显示范围
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->dateEdit->setMaximumDate(QDate::currentDate().addMonths(2));
    ui->dateEdit->setCalendarPopup(true);

    //设置表头样式
    ui->flightTabWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgba(91,155,213,1); color:white}");

    //设置当前页面为机票预订页面
    ui->stackedWidget->setCurrentWidget(ui->flight_page);
}

//初始化城市
void FlightWindow::initCity(){

    ui->fromCitycomBox->setStyleSheet("text-align:center; color:#1d4d77;");
    ui->toCitycomBox->setStyleSheet("text-align:center; color:#1d4d77;");

    /*出发城市*/
    ui->fromCitycomBox->insertItem(0,  "北京");
    ui->fromCitycomBox->insertItem(1,  "上海");
    ui->fromCitycomBox->insertItem(2,  "广州");
    ui->fromCitycomBox->insertItem(3,  "深圳");
    ui->fromCitycomBox->insertItem(4,  "成都");
    ui->fromCitycomBox->insertItem(5,  "杭州");
    ui->fromCitycomBox->insertItem(6,  "西安");
    ui->fromCitycomBox->insertItem(7,  "重庆");
    ui->fromCitycomBox->insertItem(8,  "青岛");
    ui->fromCitycomBox->insertItem(9,  "南京");
    ui->fromCitycomBox->insertItem(10, "厦门");
    ui->fromCitycomBox->insertItem(11, "昆明");
    ui->fromCitycomBox->insertItem(12, "大连");
    ui->fromCitycomBox->insertItem(13, "天津");
    ui->fromCitycomBox->insertItem(14, "郑州");
    ui->fromCitycomBox->insertItem(15, "三亚");
    ui->fromCitycomBox->insertItem(16, "济南");
    ui->fromCitycomBox->insertItem(17, "福州");

    /*目的城市*/
    ui->toCitycomBox->insertItem(0,  "北京");
    ui->toCitycomBox->insertItem(1,  "上海");
    ui->toCitycomBox->insertItem(2,  "广州");
    ui->toCitycomBox->insertItem(3,  "深圳");
    ui->toCitycomBox->insertItem(4,  "成都");
    ui->toCitycomBox->insertItem(5,  "杭州");
    ui->toCitycomBox->insertItem(6,  "西安");
    ui->toCitycomBox->insertItem(7,  "重庆");
    ui->toCitycomBox->insertItem(8,  "青岛");
    ui->toCitycomBox->insertItem(9,  "南京");
    ui->toCitycomBox->insertItem(10, "厦门");
    ui->toCitycomBox->insertItem(11, "昆明");
    ui->toCitycomBox->insertItem(12, "大连");
    ui->toCitycomBox->insertItem(13, "天津");
    ui->toCitycomBox->insertItem(14, "郑州");
    ui->toCitycomBox->insertItem(15, "三亚");
    ui->toCitycomBox->insertItem(16, "济南");
    ui->toCitycomBox->insertItem(17, "福州");
}

//给窗体添加背景图片
void FlightWindow::paintEvent(QPaintEvent *event)
{
    QPainter myPainter(this);
    myPainter.setOpacity(0.3);          //背景图片透明度
    myPainter.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/img/images/bg3.png"));
}

//点击机票预定按钮
void FlightWindow::on_flightBtn_clicked()
{
ui->stackedWidget->setCurrentWidget(ui->flight_page);
}

//点击查看订单按钮
void FlightWindow::on_orderBtn_clicked()
{
    initorder_page();
    ui->stackedWidget->setCurrentWidget(ui->order_page);
}

//点击个人中心按钮
void FlightWindow::on_myBtn_clicked()
{
ui->stackedWidget->setCurrentWidget(ui->my_page);
}

//点击航班查询按钮
void FlightWindow::on_flightQueryBtn_clicked()
{

    //设置预定按钮的样式(css文件方式)
    QFile f1;
    f1.setFileName(":/qss/css/bookbtn.css");
    f1.open(QIODevice::ReadOnly);
    QString bookbtnQss = f1.readAll();

    QString from_city = ui->fromCitycomBox->currentText();//出发城市
    QString to_city = ui->toCitycomBox->currentText();//目的城市
    QString s_date=ui->dateEdit->date().toString("yyyy-MM-dd");

    if(from_city.isEmpty() || to_city.isEmpty())
    {
        QMessageBox::about(NULL, "提示", "请选择始发地及目的城市!");
    }
    if(from_city==to_city){
        QMessageBox::about(NULL, "提示", "始发地目的城市必须不同!");
    }

    else
    {
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(QString("select * from flight_info join plane_info on flight_info.plane_code=plane_info.plane_code where from_city = '%1' and to_city = '%2' and start_date = '%3';").arg(from_city).arg(to_city).arg(s_date));
        int rowcount = model->rowCount();    // 获得行数
        if(rowcount>=1){
            for(int i = 0 ; i < rowcount; i++)
            {

           //按行读取数据
            pl_code=model->index(i,1).data().toString();
            f_code=model->index(i,0).data().toString();
            start_date=model->index(i,3).data().toString();
            start_time=model->index(i,4).data().toString();
            arrive_time=model->index(i,5).data().toDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
            f_city=model->index(i,6).data().toString();
            t_city=model->index(i,7).data().toString();
            eco_price=model->index(i,2).data().toString();
            start_port=model->index(i,8).data().toString();
            arrive_port=model->index(i,9).data().toString();

            int table_row = ui->flightTabWidget->rowCount();
            ui->flightTabWidget->insertRow(table_row);  //插入一行来盛放数据
            //数据放入表格
            ui->flightTabWidget->setItem(table_row, 0, new QTableWidgetItem(pl_code));
            ui->flightTabWidget->setItem(table_row, 1, new QTableWidgetItem(f_code));
            ui->flightTabWidget->setItem(table_row, 2, new QTableWidgetItem(start_date));
            ui->flightTabWidget->setItem(table_row, 3, new QTableWidgetItem(start_time));
            ui->flightTabWidget->setItem(table_row, 4, new QTableWidgetItem(arrive_time));
            ui->flightTabWidget->setItem(table_row, 5, new QTableWidgetItem(f_city));
            ui->flightTabWidget->setItem(table_row, 6, new QTableWidgetItem(t_city));
            ui->flightTabWidget->setItem(table_row, 7, new QTableWidgetItem(eco_price));
            ui->flightTabWidget->setItem(table_row, 8, new QTableWidgetItem(start_port));
            ui->flightTabWidget->setItem(table_row, 9, new QTableWidgetItem(arrive_port));

            //添加按钮
            QPushButton *bookBtn = new QPushButton("预定");
            bookBtn->setObjectName("Bookbtn"+ QString::number(i));
            bookBtn->setFixedSize(90,30);
            bookBtn->setStyleSheet(bookbtnQss);//添加样式
            ui->flightTabWidget->setCellWidget(i, 10,bookBtn);

            //设置信号与槽
            connect(bookBtn,SIGNAL(clicked()),this,SLOT(book_flight()));
            }
        }
        else
        {
            QMessageBox::about(NULL, "提示", "未查询到城市相关航班信息!");
           clear_flightTabWidget();
        }
    }
    f1.close();//关闭样式文件
    qDebug()<<from_city<<"\n"<<to_city<<"\n"<<s_date;

}

//清理flightTabWidget
void FlightWindow::clear_flightTabWidget()
{
    ui->flightTabWidget->clearContents();
    ui->flightTabWidget->setRowCount(0);
}

//点击某一行的预定按钮
void FlightWindow::book_flight(){

    int row=ui->flightTabWidget->currentRow();//获取当前行
    u_tel2=u_tel;
    u_name2=u_name;
    u_IDCard2=u_IDCard;
    selected_f_code = ui->flightTabWidget->item(row,1)->text();
    selected_f_city = ui->flightTabWidget->item(row,5)->text();
    selected_t_city = ui->flightTabWidget->item(row,6)->text();
    selected_s_date = ui->flightTabWidget->item(row,2)->text();
    selected_s_time = ui->flightTabWidget->item(row,3)->text();

    //弹出对话框
    bookDialog book;
    book.setWindowTitle("填写预定信息");
    book.setModal(false);
    book.exec();

}

//设置菜单按钮样式
void FlightWindow::set_menuBtnStyle(){
     ui->groupBox->setStyleSheet("border:1px solid #1d4d77;");
     ui->flightBtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
     ui->orderBtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
     ui->myBtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
     ui->flightQueryBtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
}

/***********************订单页面相关***************************************/

//初始化订单界面
void FlightWindow::initorder_page(){

    //设置控件样式
    ui->orderTabWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgba(91,155,213,1); color:white}");
    ui->orderQueryBtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->keyLinedit->setStyleSheet("color:#1d4d77; font-size:16px;");

    //初始化订单信息

}
//查询订单
void FlightWindow::on_searchpushBtn_clicked()
{

}

