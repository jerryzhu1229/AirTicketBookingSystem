#include "flightwindow.h"
#include "ui_flightwindow.h"
#include "bookdialog.h"
#include "alterdialog.h"
#include "MainWindow.h"
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QCryptographicHash>
#include <QTimer>
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

//这是alterDialog要用到的信息
extern QString old_fr_city;  //改签前出发城市 改签后不变
extern QString old_to_city; //改签前目的城市 改签后不变
extern QString old_order_code; //改签前订单编号 改签后不变
extern QString old_flight_code; //改签前航班编号 改签后会变
extern QString old_seat_code; //改签前座位编号 改签后会变
extern QString old_passger_tel; //改签前乘客电话 改签后不变
extern QString old_passger_IDCard; //改签前乘客身份证号 改签后不变
FlightWindow::FlightWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlightWindow)
{
    ui->setupUi(this);
    //设置窗口图标
    this->setWindowIcon(QIcon(":/img/images/winicon.png"));
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
    ui->msgLabel->setVisible(false);
    ui->iconlab->setVisible(false);

    //设置欢迎信息
    QString welmsg=u_name+",您好,欢迎使用航空机票预订系统》》》";
    ui->welMsgLabel->setText(welmsg);
    ui->welMsgLabel->setStyleSheet("color:#1d4d77");

    //行程提醒
    QDateTime local(QDateTime::currentDateTime());
    QString localtime=local.toString("yyyy-MM-dd hh:mm:ss");
    QDateTime current_time=QDateTime::fromString(localtime,"yyyy-MM-dd hh:mm:ss");
    //获取待出行订单信息
    QString orderQuery=QString("select * from order_info join flight_info on order_info.flight_code=flight_info.flight_code \
                               where order_info.passger_tel='%1' and order_status='已支付' order by start_date")
                              .arg(u_tel);
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(orderQuery);
    int rowcount = model->rowCount();    // 获得行数
    if(rowcount>=1){
        for(int i = 0 ; i < rowcount; i++)
        {
            //按行读取数据

             QString flight_code=model->index(i,1).data().toString();
             QString fr_city=model->index(i,14).data().toString();
             QString to_city=model->index(i,15).data().toString();
             QString st_date=model->index(i,11).data().toString();
             QString st_time=model->index(i,12).data().toString();
             QString s_time= st_date+" "+st_time;
             QDateTime flight_time=QDateTime::fromString(s_time,"yyyy-MM-dd hh:mm:ss");

             //当前时间与机票出发时间间隔的小时数
             int hours = (current_time.secsTo(flight_time)/60/60);
             QString str_hours = QString("%1").arg(hours);
             //如果间隔不足50小时，设置消息提醒
            if(hours<=50){
                QString alert="尊敬的乘客，您预定的由"+fr_city+"飞往"+to_city+"的"+flight_code+"次航班将于"+str_hours+"小时后出发，请注意时间，祝旅途愉快!";
                //设置文字走马灯效果
                timer = new QTimer();
                ui->msgLabel->clear();
                ui->msgLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                text=alert;
                text_width=fontMetrics().width(text);
                d_text_width=0;
                d_index=0;
                space_count=0;
                //通过定时器 定时修改显示文本
                connect(timer,SIGNAL(timeout()),this,SLOT(updateIndex()));
                timer->start(260);
                ui->msgLabel->setStyleSheet("color:#d2691e;");
                ui->msgLabel->setVisible(true);
                ui->iconlab->setVisible(true);
                break;
            }
        }
    }
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

//计时器触发事件
void FlightWindow::updateIndex(){
    if(d_text_width-text_width > ui->msgLabel->size().width()){//当文本像素宽度大于label的宽度时，则重新初始化
            ui->msgLabel->clear();
            d_text.clear();
            d_index=0;
            d_text_width=0;
        }

        if(space_count>=10){//当填充的空格大于10个时，开始继续填充文本
            space_count=0;
            d_index=0;
        }

        if(d_index < text.length()){
            d_text+=text.at(d_index);
            d_index++;
        }
        else{
            d_text += " ";
            space_count++;
        }

        d_text_width=fontMetrics().width(d_text);

        //    qDebug()<<d_text_width<<" "<< text_width<<" "<<d_index<<" "<<d_text;
        ui->msgLabel->setText(d_text);

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
    //改变按钮样式
     QFile f1;
     f1.setFileName(":/qss/css/menubtn.css");
     f1.open(QIODevice::ReadOnly);
     QString menubtnQss = f1.readAll();
     //ui->groupBox->setStyleSheet("border:1px solid #1d4d77;");
     // ui->flightBtn->setStyleSheet(menubtnQss);
     ui->orderBtn->setStyleSheet(menubtnQss);
     ui->myBtn->setStyleSheet(menubtnQss);
     QFile f2;
     f2.setFileName(":/qss/css/activebtn.css");
     f2.open(QIODevice::ReadOnly);
     QString activebtnQss = f2.readAll();
     ui->flightBtn->setStyleSheet(activebtnQss);
     f2.close();

     ui->stackedWidget->setCurrentWidget(ui->flight_page);
}

//点击查看订单按钮
void FlightWindow::on_orderBtn_clicked()
{
    //改变按钮样式
     QFile f1;
     f1.setFileName(":/qss/css/menubtn.css");
     f1.open(QIODevice::ReadOnly);
     QString menubtnQss = f1.readAll();
     //ui->groupBox->setStyleSheet("border:1px solid #1d4d77;");
     ui->flightBtn->setStyleSheet(menubtnQss);
     //ui->orderBtn->setStyleSheet(menubtnQss);
     ui->myBtn->setStyleSheet(menubtnQss);
     QFile f2;
     f2.setFileName(":/qss/css/activebtn.css");
     f2.open(QIODevice::ReadOnly);
     QString activebtnQss = f2.readAll();
     ui->orderBtn->setStyleSheet(activebtnQss);
     f2.close();

     initorder_page();
     ui->stackedWidget->setCurrentWidget(ui->order_page);
}

//点击个人中心按钮
void FlightWindow::on_myBtn_clicked()
{
    //改变按钮样式
     QFile f1;
     f1.setFileName(":/qss/css/menubtn.css");
     f1.open(QIODevice::ReadOnly);
     QString menubtnQss = f1.readAll();
     //ui->groupBox->setStyleSheet("border:1px solid #1d4d77;");
     ui->flightBtn->setStyleSheet(menubtnQss);
     ui->orderBtn->setStyleSheet(menubtnQss);
     //ui->myBtn->setStyleSheet(menubtnQss);
     QFile f2;
     f2.setFileName(":/qss/css/activebtn.css");
     f2.open(QIODevice::ReadOnly);
     QString activebtnQss = f2.readAll();
     ui->myBtn->setStyleSheet(activebtnQss);
     f2.close();
    innimy_page();
    ui->stackedWidget->setCurrentWidget(ui->my_page);
}

//点击航班查询按钮
void FlightWindow::on_flightQueryBtn_clicked()
{
    //先清空之前的航班信息
    clear_flightTabWidget();

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
        model->setQuery(QString("select * from flight_info join plane_info on flight_info.plane_code=plane_info.plane_code where flight_status = '未发机' and from_city = '%1' and to_city = '%2' and start_date = '%3' order by start_time;").arg(from_city).arg(to_city).arg(s_date));
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
    //设置预定按钮的样式(css文件方式)
    QFile f1;
    f1.setFileName(":/qss/css/menubtn.css");
    f1.open(QIODevice::ReadOnly);
    QString menubtnQss = f1.readAll();
     ui->groupBox->setStyleSheet("border:1px solid #1d4d77;");
     ui->flightBtn->setStyleSheet(menubtnQss);
     ui->orderBtn->setStyleSheet(menubtnQss);
     ui->myBtn->setStyleSheet(menubtnQss);
     ui->flightQueryBtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
     f1.close();
}

/***********************订单页面相关********************************************/

//初始化订单界面
void FlightWindow::initorder_page(){

    //先清空
    clear_orderTabWidget();

    //设置控件样式
    ui->orderTabWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgba(91,155,213,1); color:white}");
    ui->orderQueryBtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    ui->keyLinedit->setStyleSheet("color:#1d4d77; font-size:16px;");

    //设置按钮的样式(css文件方式)
    QFile f1;
    f1.setFileName(":/qss/css/bookbtn.css");
    f1.open(QIODevice::ReadOnly);
    QString bookbtnQss = f1.readAll();

    ui->orderTabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->orderTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置每行不可编辑
    //初始化订单信息
    QString orderQuery=QString("select * from order_info join flight_info on order_info.flight_code=flight_info.flight_code  \
                               join passger_info on order_info.passger_tel=passger_info.u2_tel \
                               where order_info.booku_tel='%1' and passger_info.u1_tel='%1' order by create_time desc")
                               .arg(u_tel);
    QSqlQueryModel *model = new QSqlQueryModel;
     model->setQuery(orderQuery);
    int rowcount = model->rowCount();    // 获得行数
    if(rowcount>=1){
        for(int i = 0 ; i < rowcount; i++)
        {
            //按行读取数据
             order_code=model->index(i,0).data().toString();
             flight_code=model->index(i,1).data().toString();
             seat_code=model->index(i,2).data().toString();
             pass_name=model->index(i,23).data().toString();
             pass_tel=model->index(i,4).data().toString();
             fr_city=model->index(i,14).data().toString();
             s_port=model->index(i,16).data().toString();
             to_city=model->index(i,15).data().toString();
             to_port=model->index(i,17).data().toString();
             s_date=model->index(i,11).data().toString();
             s_time=model->index(i,12).data().toString();
             order_total=model->index(i,5).data().toString().toInt();
             order_status=model->index(i,6).data().toString();

             int table_row = ui->orderTabWidget->rowCount();
             ui->orderTabWidget->insertRow(table_row);  //插入一行来盛放数据
             //数据放入表格
             ui->orderTabWidget->setItem(table_row, 0, new QTableWidgetItem(order_code));
             ui->orderTabWidget->setItem(table_row, 1, new QTableWidgetItem(flight_code));
             ui->orderTabWidget->setItem(table_row, 2, new QTableWidgetItem(seat_code));
             ui->orderTabWidget->setItem(table_row, 3, new QTableWidgetItem(pass_name));
             ui->orderTabWidget->setItem(table_row, 4, new QTableWidgetItem(pass_tel));
             ui->orderTabWidget->setItem(table_row, 5, new QTableWidgetItem(fr_city+" "+s_port));
             ui->orderTabWidget->setItem(table_row, 6, new QTableWidgetItem(to_city+" "+to_port));
             ui->orderTabWidget->setItem(table_row, 7, new QTableWidgetItem(s_date));
             ui->orderTabWidget->setItem(table_row, 8, new QTableWidgetItem(s_time));
             ui->orderTabWidget->setItem(table_row, 9, new QTableWidgetItem(model->index(i,5).data().toString()));
             ui->orderTabWidget->setItem(table_row, 10, new QTableWidgetItem(order_status));

             //添加退票按钮
             QPushButton *cancelBtn = new QPushButton("退票");
             cancelBtn->setObjectName("cancelBtn"+ QString::number(i));
             cancelBtn->setFixedSize(90,30);
             cancelBtn->setStyleSheet(bookbtnQss);//添加样式
             ui->orderTabWidget->setCellWidget(i, 11,cancelBtn);
             //设置信号与槽
             connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancel_flight()));

             //添加改签按钮
             QPushButton *alterBtn = new QPushButton("改签");
             alterBtn->setObjectName("alterBtn"+ QString::number(i));
             alterBtn->setFixedSize(90,30);
             alterBtn->setStyleSheet(bookbtnQss);//添加样式
             ui->orderTabWidget->setCellWidget(i, 12,alterBtn);
             //设置信号与槽
             connect(alterBtn,SIGNAL(clicked()),this,SLOT(alter_flight()));

             ui->orderTabWidget->setItem(table_row, 13, new QTableWidgetItem(fr_city));
             ui->orderTabWidget->setItem(table_row, 14, new QTableWidgetItem(to_city));
             ui->orderTabWidget->setItem(table_row, 15, new QTableWidgetItem(model->index(i,25).data().toString())); //乘客身份证
        }

        }
    else
    {
             QMessageBox::about(NULL, "提示", "未查询到订单信息!");
             clear_orderTabWidget();
    }
             f1.close();//关闭样式文件

             ui->orderTabWidget->setColumnHidden(13, true); // 隐藏第13列
             ui->orderTabWidget->setColumnHidden(14, true); // 隐藏第14列
             ui->orderTabWidget->setColumnHidden(15, true); // 隐藏第15列
}

//清空订单表信息
void FlightWindow::clear_orderTabWidget(){
    ui->orderTabWidget->clearContents();
    ui->orderTabWidget->setRowCount(0);
}

//查询订单
void FlightWindow::on_orderQueryBtn_clicked()
{

         //先清空
         clear_orderTabWidget();
         //设置按钮的样式(css文件方式)
         QFile f1;
         f1.setFileName(":/qss/css/bookbtn.css");
         f1.open(QIODevice::ReadOnly);
         QString bookbtnQss = f1.readAll();
         ui->orderTabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
         ui->orderTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置每行不可编辑
         //获得查询关键字
         QString key_word=ui->keyLinedit->text();
         QString query=QString("select * from order_info join flight_info on order_info.flight_code=flight_info.flight_code  \
                               join passger_info on order_info.passger_tel=passger_info.u2_tel \
                               where order_info.booku_tel='%1' and passger_info.u1_tel='%1' \
                               and (order_info.order_code like '%"+key_word+"%' \
                               or flight_info.flight_code like '%"+key_word+"%' or passger_info.u2_name like '%"+key_word+"%') order by create_time desc")
                               .arg(u_tel);
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(query);
        int rowcount = model->rowCount();    // 获得行数
        if(rowcount>=1){
         for(int i = 0 ; i < rowcount; i++)
         {
                         //按行读取数据
            order_code=model->index(i,0).data().toString();
            flight_code=model->index(i,1).data().toString();
            seat_code=model->index(i,2).data().toString();
            pass_name=model->index(i,23).data().toString();
            pass_tel=model->index(i,4).data().toString();
            fr_city=model->index(i,14).data().toString();
            s_port=model->index(i,16).data().toString();
            to_city=model->index(i,15).data().toString();
            to_port=model->index(i,17).data().toString();
            s_date=model->index(i,11).data().toString();
            s_time=model->index(i,12).data().toString();
            order_total=model->index(i,5).data().toString().toInt();
            order_status=model->index(i,6).data().toString();

            int table_row = ui->orderTabWidget->rowCount();
            ui->orderTabWidget->insertRow(table_row);  //插入一行来盛放数据
            //数据放入表格
            ui->orderTabWidget->setItem(table_row, 0, new QTableWidgetItem(order_code));
            ui->orderTabWidget->setItem(table_row, 1, new QTableWidgetItem(flight_code));
            ui->orderTabWidget->setItem(table_row, 2, new QTableWidgetItem(seat_code));
            ui->orderTabWidget->setItem(table_row, 3, new QTableWidgetItem(pass_name));
            ui->orderTabWidget->setItem(table_row, 4, new QTableWidgetItem(pass_tel));
            ui->orderTabWidget->setItem(table_row, 5, new QTableWidgetItem(fr_city+" "+s_port));
            ui->orderTabWidget->setItem(table_row, 6, new QTableWidgetItem(to_city+" "+to_port));
            ui->orderTabWidget->setItem(table_row, 7, new QTableWidgetItem(s_date));
            ui->orderTabWidget->setItem(table_row, 8, new QTableWidgetItem(s_time));
            ui->orderTabWidget->setItem(table_row, 9, new QTableWidgetItem(model->index(i,5).data().toString()));
            ui->orderTabWidget->setItem(table_row, 10, new QTableWidgetItem(order_status));

            //添加退票按钮
            QPushButton *cancelBtn = new QPushButton("退票");
            cancelBtn->setObjectName("cancelBtn"+ QString::number(i));
            cancelBtn->setFixedSize(90,30);
            cancelBtn->setStyleSheet(bookbtnQss);//添加样式
            ui->orderTabWidget->setCellWidget(i, 11,cancelBtn);
            //设置信号与槽
            connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancel_flight()));

            //添加改签按钮
            QPushButton *alterBtn = new QPushButton("改签");
            alterBtn->setObjectName("alterBtn"+ QString::number(i));
            alterBtn->setFixedSize(90,30);
            alterBtn->setStyleSheet(bookbtnQss);//添加样式
            ui->orderTabWidget->setCellWidget(i, 12,alterBtn);
            //设置信号与槽
            connect(alterBtn,SIGNAL(clicked()),this,SLOT(alter_flight()));

            ui->orderTabWidget->setItem(table_row, 13, new QTableWidgetItem(fr_city));
            ui->orderTabWidget->setItem(table_row, 14, new QTableWidgetItem(to_city));
            ui->orderTabWidget->setItem(table_row, 15, new QTableWidgetItem(model->index(i,25).data().toString())); //乘客身份证

            }
         }
        else
        {
            QMessageBox::about(NULL, "提示", "未查询到订单信息!");
            clear_orderTabWidget();
         }
        ui->orderTabWidget->setColumnHidden(13, true); // 隐藏第13列
        ui->orderTabWidget->setColumnHidden(14, true); // 隐藏第14列
        ui->orderTabWidget->setColumnHidden(15, true); // 隐藏第15列
}

//点击某一行的退票按钮
void FlightWindow::cancel_flight(){

    QMessageBox::StandardButton box;
    box = QMessageBox::question(this, "提示", "确定要退票吗?", QMessageBox::Yes|QMessageBox::No);
    if(box==QMessageBox::No)
          return;
    else{
         int  row=ui->orderTabWidget->currentRow();//获取当前行
         QString order_code= ui->orderTabWidget->item(row,0)->text();
         QString flight_code= ui->orderTabWidget->item(row,1)->text();
         QString seat_code= ui->orderTabWidget->item(row,2)->text();

         QDateTime local(QDateTime::currentDateTime());
         QString localtime=local.toString("yyyy-MM-dd hh:mm:ss");
         QDateTime current_time=QDateTime::fromString(localtime,"yyyy-MM-dd hh:mm:ss");
         QString s_time= ui->orderTabWidget->item(row,7)->text()+" "+ui->orderTabWidget->item(row,8)->text();
         QDateTime flight_time=QDateTime::fromString(s_time,"yyyy-MM-dd hh:mm:ss");

         //当前时间与机票出发时间间隔的小时数
         int hours = current_time.secsTo(flight_time)/60/60;

         //如果间隔不足24小时，不允许退票
        if(hours<24){
            QMessageBox::about(NULL, "提示", "距离出发时间不足24小时，退订失败!");
        }
        else{
         QSqlQuery query1;
         QSqlQuery query2;
         //第一步：从订单表中删除
         QString del_query=QString("delete from order_info where order_code='%1'").arg(order_code);
         //第二步：更新机票信息
         QString update_query=QString("update ticket_info set passger_IDCard = '0' where flight_code = '%1' and seat_code = '%2'").arg(flight_code).arg(seat_code);
         if(!(query1.exec(del_query) && query2.exec(update_query))){
             QMessageBox::about(NULL, "提示", "退订失败!");
         }
         initorder_page(); //重新显示订单表
         qDebug()<<order_code<<" "<<flight_code<<" "<<seat_code;
        }

   }
}

//改签航班
void FlightWindow::alter_flight(){
    int  row=ui->orderTabWidget->currentRow();//获取当前行
    //获取改签前的订单信息
    old_order_code= ui->orderTabWidget->item(row,0)->text();
    old_flight_code= ui->orderTabWidget->item(row,1)->text();
    old_seat_code= ui->orderTabWidget->item(row,2)->text();
    old_fr_city=ui->orderTabWidget->item(row,13)->text();
    old_to_city=ui->orderTabWidget->item(row,14)->text();
    old_passger_tel=ui->orderTabWidget->item(row,4)->text();
    old_passger_IDCard=ui->orderTabWidget->item(row,15)->text();

    //弹出改签对话框
    alterdialog alter;
    alter.setWindowTitle("改签航班");
    alter.setModal(false);
    alter.exec();
}

/***********************个人中心相关**********************************************/

//初始化待出行订单界面
void FlightWindow::innimy_page(){
             ui->msglab->setVisible(false);
             //设置控件样式
             ui->myorderTabWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgba(91,155,213,1); color:white}");
             ui->show_orderbtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(29,77,119,0.7); font-size:16px; color:white");
             ui->show_passgerbtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(29,77,119,0.7); font-size:16px; color:white");
             ui->alter_passbtn->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(29,77,119,0.7); font-size:16px; color:white");
             //先清空待出行订单
             clear_myorderTabWidget();
             ui->myorderTabWidget->horizontalHeader()->setStretchLastSection(true);
             ui->myorderTabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
             ui->myorderTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置每行不可编辑
             //初始化待出行订单信息
             QString orderQuery=QString("select * from order_info join flight_info on order_info.flight_code=flight_info.flight_code \
                                        where order_info.passger_tel='%1' and order_status='已支付' order by create_time desc")
                                       .arg(u_tel);
             QSqlQueryModel *model = new QSqlQueryModel;
             model->setQuery(orderQuery);
             int rowcount = model->rowCount();    // 获得行数
             if(rowcount>=1){
                 ui->myorderTabWidget->setVisible(true);
                 for(int i = 0 ; i < rowcount; i++)
                 {
                     //按行读取数据
                      order_code=model->index(i,0).data().toString();
                      flight_code=model->index(i,1).data().toString();
                      seat_code=model->index(i,2).data().toString();
                      fr_city=model->index(i,14).data().toString();
                      s_port=model->index(i,16).data().toString();
                      to_city=model->index(i,15).data().toString();
                      to_port=model->index(i,17).data().toString();
                      s_date=model->index(i,11).data().toString();
                      s_time=model->index(i,12).data().toString();
                      order_status=model->index(i,6).data().toString();

                      int table_row = ui->myorderTabWidget->rowCount();
                      ui->myorderTabWidget->insertRow(table_row);  //插入一行来盛放数据

                      //数据放入表格
                      ui->myorderTabWidget->setItem(table_row, 0, new QTableWidgetItem(order_code));
                      ui->myorderTabWidget->setItem(table_row, 1, new QTableWidgetItem(flight_code));
                      ui->myorderTabWidget->setItem(table_row, 2, new QTableWidgetItem(seat_code));
                      ui->myorderTabWidget->setItem(table_row, 3, new QTableWidgetItem(fr_city+" "+s_port));
                      ui->myorderTabWidget->setItem(table_row, 4, new QTableWidgetItem(to_city+" "+to_port));
                      ui->myorderTabWidget->setItem(table_row, 5, new QTableWidgetItem(s_date));
                      ui->myorderTabWidget->setItem(table_row, 6, new QTableWidgetItem(s_time));
                      ui->myorderTabWidget->setItem(table_row, 7, new QTableWidgetItem(order_status));
                 }

             }else{
                 ui->myorderTabWidget->setVisible(false);
                 ui->msglab->setVisible(true);
             }

}

//初始化乘客信息
void FlightWindow::innipassengers_page(){
    clear_passTabWidget();
    QFile f1;
    f1.setFileName(":/qss/css/bookbtn.css");
    f1.open(QIODevice::ReadOnly);
    QString bookbtnQss = f1.readAll();
    ui->msglab2->setVisible(false);
    //设置控件样式
    ui->passTabWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgba(91,155,213,1); color:white}");
    ui->passTabWidget->horizontalHeader()->setStretchLastSection(true);
    ui->passTabWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->add_pushButton->setStyleSheet("border:1px dotted #1d4d77; border-radius:15px; background-color:rgba(91,155,213,1); color:white");
    //初始化待出行订单信息
    QString passengerQuery=QString("select * from passger_info where u1_tel='%1'")
                              .arg(u_tel);
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(passengerQuery);
    int rowcount = model->rowCount();    // 获得行数
    if(rowcount>=1){
        ui->passTabWidget->setVisible(true);
        for(int i = 0 ; i < rowcount; i++)
        {
            //按行读取数据
             passenger_name=model->index(i,4).data().toString();
             passenger_tel=model->index(i,5).data().toString();
             passenger_IDCard=model->index(i,6).data().toString();
             int table_row = ui->passTabWidget->rowCount();
             ui->passTabWidget->insertRow(table_row);  //插入一行来盛放数据

             //数据放入表格
             ui->passTabWidget->setItem(table_row, 0, new QTableWidgetItem(passenger_name));
             ui->passTabWidget->setItem(table_row, 1, new QTableWidgetItem(passenger_tel));
             ui->passTabWidget->setItem(table_row, 2, new QTableWidgetItem(passenger_IDCard));
             ui->passTabWidget->setItem(table_row, 5, new QTableWidgetItem(model->index(i,0).data().toString()));

             //添加修改按钮
             QPushButton *save_alter_Btn = new QPushButton("修改");
             save_alter_Btn->setObjectName("save_alter_Btn"+ QString::number(i));
             save_alter_Btn->setFixedSize(90,30);
             save_alter_Btn->setStyleSheet(bookbtnQss);//添加样式
             ui->passTabWidget->setCellWidget(i, 3,save_alter_Btn);
             //设置信号与槽
             connect(save_alter_Btn,SIGNAL(clicked()),this,SLOT(alter_passenger()));

             //添加删除按钮
             QPushButton *delBtn = new QPushButton("删除");
             delBtn->setObjectName("alterBtn"+ QString::number(i));
             delBtn->setFixedSize(90,30);
             delBtn->setStyleSheet(bookbtnQss);//添加样式
             ui->passTabWidget->setCellWidget(i, 4,delBtn);
             //设置信号与槽
             connect(delBtn,SIGNAL(clicked()),this,SLOT(del_passenger()));
             ui->passTabWidget->setColumnHidden(5, true);
        }

    }else{
        ui->passTabWidget->setVisible(false);
        ui->msglab2->setVisible(true);
    }
    f1.close();
}

//初始化修改密码页面
void FlightWindow::innialterpass_page(){
    ui->groupBox_2->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->new1Edit->setVisible(false);
    ui->new2Edit->setVisible(false);
    ui->confirm_alterbtn->setVisible(false);
    ui->oldpassEdit->setEchoMode(QLineEdit::Password);
    ui->new1Edit->setEchoMode(QLineEdit::Password);
    ui->new2Edit->setEchoMode(QLineEdit::Password);
    //设置控件样式
    QFile f1;
    f1.setFileName(":/qss/css/bookbtn.css");
    f1.open(QIODevice::ReadOnly);
    QString bookbtnQss = f1.readAll();
    ui->confirm_alterbtn->setStyleSheet(bookbtnQss);
    ui->nextbtn->setStyleSheet(bookbtnQss);
    ui->groupBox_2->setStyleSheet("border:1px solid #1d4d77; background-color:rgba(248,248,255,0.5);");
    ui->label_7->setStyleSheet("border:none; background-color:transparent;");
    ui->label_8->setStyleSheet("border:none; background-color:transparent;");
    ui->new1Edit->setStyleSheet("border-color:gray;");
    ui->new2Edit->setStyleSheet("border-color:gray;");

}

//点击待出行订单按钮
void FlightWindow::on_show_orderbtn_clicked(){
             innimy_page();
             ui->stackedWidget_2->setCurrentWidget(ui->readyorder_page);

}

//点击乘客列表按钮
void FlightWindow::on_show_passgerbtn_clicked()
{
    innipassengers_page();
    ui->stackedWidget_2->setCurrentWidget(ui->passengers_page);
}

//点击修改密码按钮
void FlightWindow::on_alter_passbtn_clicked()
{
    innialterpass_page();
  ui->stackedWidget_2->setCurrentWidget(ui->alterpass_page);
}

//清空待出行订单表格
void FlightWindow::clear_myorderTabWidget(){
             ui->myorderTabWidget->clearContents();
             ui->myorderTabWidget->setRowCount(0);
}

//清空乘客表格
void FlightWindow::clear_passTabWidget(){
    ui->passTabWidget->clearContents();
    ui->passTabWidget->setRowCount(0);
}

//添加乘客
void FlightWindow::on_add_pushButton_clicked()
{
    QString p_name=ui->name_lineEdit->text();
    QString p_tel=ui->tel_lineEdit->text();
    QString p_IDCard=ui->ID_lineEdit->text();
    if(p_name.isEmpty()||p_tel.isEmpty()||p_IDCard.isEmpty()){
        QMessageBox::about(NULL, "提示", "请填写完整信息!");
       }else{

              QSqlQuery query;
              QString add_passenger=QString("insert into passger_info(u1_name,u1_tel,u1_IDCard,u2_name,u2_tel,u2_IDCard)   \
                                               values('%1', '%2', '%3', '%4', '%5', '%6')")                                                                  \
                                               .arg(u_name).arg(u_tel).arg(u_IDCard).arg(p_name).arg(p_tel).arg(p_IDCard);

              if(query.exec(add_passenger)){

                 innipassengers_page();
            }
        }
}

//修改乘客
void FlightWindow::alter_passenger(){
             int  row=ui->passTabWidget->currentRow();//获取当前行
             QString p_id=ui->passTabWidget->item(row,5)->text();
             QString p_name=ui->passTabWidget->item(row,0)->text();
             QString p_tel=ui->passTabWidget->item(row,1)->text();
             QString p_IDCard=ui->passTabWidget->item(row,2)->text();
             if(p_name.isEmpty()||p_tel.isEmpty()||p_IDCard.isEmpty()){
                 QMessageBox::about(NULL, "提示", "请填写完整信息!");
                }else{
                 QMessageBox::StandardButton box;
                 box = QMessageBox::question(this, "提示", "确定要修改吗?", QMessageBox::Yes|QMessageBox::No);
                    if(box==QMessageBox::No)
                       return;
                    else{
                       QSqlQuery query;
                       QString update_passenger=QString("update passger_info set u2_name='%1',u2_tel='%2',u2_IDCard='%3' where id='%4'")
                                    .arg(p_name).arg(p_tel).arg(p_IDCard).arg(p_id);
                       if(query.exec(update_passenger)){
                          QMessageBox::about(NULL, "提示", "修改成功!");
                          innipassengers_page();
            }
        }
    }

}

//删除乘客
void FlightWindow::del_passenger(){
    int  row=ui->passTabWidget->currentRow();//获取当前行
    QString p_id=ui->passTabWidget->item(row,5)->text();
    QMessageBox::StandardButton box;
    box = QMessageBox::question(this, "提示", "确定要删除吗?", QMessageBox::Yes|QMessageBox::No);
       if(box==QMessageBox::No)
          return;
       else{
          QSqlQuery query2;
          QString del_passenger=QString("delete from passger_info where id='%4'").arg(p_id);
          if(query2.exec(del_passenger)){
             QMessageBox::about(NULL, "提示", "删除成功!");
             innipassengers_page();
}
}
}

//点击下一步按钮
void FlightWindow::on_nextbtn_clicked()
{
    QString pass1=ui->oldpassEdit->text();
    QByteArray byte_password = pass1.toLatin1();
    QByteArray byte_password_md5 = QCryptographicHash::hash(byte_password,QCryptographicHash::Md5);//使用哈希映射加密
    QString strPwdMd5 = byte_password_md5.toHex();//存在数据库里的加密密码
    QString query=QString("select * from user_info where u_tel='%1' and u_pwd='%2'").arg(u_tel).arg(strPwdMd5);
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(query);
    int rowcount=model->rowCount();
    if(rowcount==1){
        ui->groupBox_2->setVisible(true);
        ui->label_7->setVisible(true);
        ui->label_8->setVisible(true);
        ui->new1Edit->setVisible(true);
        ui->new2Edit->setVisible(true);
        ui->confirm_alterbtn->setVisible(true);
    }else{
        QMessageBox::about(NULL, "提示", "原密码输入错误!");
    }
}

//点击提交修改密码按钮
void FlightWindow::on_confirm_alterbtn_clicked()
{
   QString p1=ui->new1Edit->text();
   QString p2=ui->new2Edit->text();
   if(p1!=p2){
       QMessageBox::about(NULL, "提示", "两次密码输入不一致!");
   }else {
       QByteArray byte_password = p1.toLatin1();
       QByteArray byte_password_md5 = QCryptographicHash::hash(byte_password,QCryptographicHash::Md5);
       QString strPwdMd5 = byte_password_md5.toHex();
       QByteArray b_password_md5 = strPwdMd5.toLatin1();
       char *c_password_md5 = b_password_md5.data();
       QSqlQuery query;
       QString update_pass=QString("update user_info set u_pwd='%1' where u_tel='%2'").arg(c_password_md5).arg(u_tel);
       if(query.exec(update_pass)){
          QMessageBox::about(NULL, "提示", "修改成功，请重新登录!");
          this->close();
          MainWindow *w = new MainWindow;
          w->show();
       }
   }
}


//安全退出
void FlightWindow::on_pushButton_clicked()
{
    this->close();
    MainWindow *w = new MainWindow;
    w->show();
}

