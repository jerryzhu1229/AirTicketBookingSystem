#ifndef FLIGHTWINDOW_H
#define FLIGHTWINDOW_H
#include <QWidget>

namespace Ui {
class FlightWindow;
}

class FlightWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FlightWindow(QWidget *parent = nullptr);
    ~FlightWindow();

private:
    Ui::FlightWindow *ui;

    //航班相关
    QString pl_code;//飞机编号
    QString f_code;//航班编号
    QString start_date;//出发日期
    QString start_time;//出发时间
    QString arrive_time;//到达时间
    QString f_city;//出发城市
    QString t_city;//目的城市
    QString eco_price;//最低票价
    QString start_port;//始发机场
    QString arrive_port;//目的机场

    //订单相关
    QString order_code;//订单编号
    QString flight_code;//航班编号
    QString seat_code;//座位编号
    QString pass_name;//乘客姓名
    QString pass_tel;//乘客电话
    QString fr_city;//出发城市
    QString s_port;//出发机场
    QString to_city;//目的城市
    QString to_port;//目的机场
    QString s_date;//出发日期
    QString s_time;//出发时间
    int order_total;//订单小计
    QString order_status;//订单状态

    //乘客相关
    QString passenger_name;//乘客姓名
    QString passenger_tel;//乘客电话
    QString passenger_IDCard;//乘客身份证

    //跑马灯效果
    /*方式一：通过定时器实现 */
    QString text;//需要显示的文本
    QString d_text;//正在显示的文本
    QTimer* timer;//定时器
    int text_width;//需要显示的文本宽度
    int d_text_width;//正在显示的文本宽度
    int d_index;//填充文本索引
    int space_count;//填充空格计数

private slots:
    void initUser_info();  //初始化用户信息
    void initFlightWindow();//初始化界面信息
    void initCity(); //初始化城市信息
    void paintEvent(QPaintEvent *event);  //为窗体添加背景图片
    void on_flightBtn_clicked();  //跳转到航班预订界面
    void on_orderBtn_clicked();  //跳转到查看界面
    void on_myBtn_clicked();  //跳转到个人中心界面
    void on_flightQueryBtn_clicked();  //查询航班
    void book_flight();//预定航班
    void clear_flightTabWidget();//清空航班表格信息
    void set_menuBtnStyle();//设置菜单按钮样式

    void initorder_page();//初始化订单界面
    void clear_orderTabWidget();//清空订单表信息
    void on_orderQueryBtn_clicked();//查询订单
    void cancel_flight();//取消航班
    void alter_flight();//改签航班
    void innimy_page();//初始化个人中心界面
    void innipassengers_page();//初始化乘客信息界面
    void on_show_orderbtn_clicked();//点击待出行订单
    void on_show_passgerbtn_clicked();//点击乘客列表
    void on_alter_passbtn_clicked();//点击修改密码
    void clear_myorderTabWidget();//清空待出行订单表格
    void clear_passTabWidget();//清空乘客信息表格
    void on_add_pushButton_clicked();//添加乘客
    void alter_passenger();//修改乘客信息
    void del_passenger();//删除乘客信息
    void on_nextbtn_clicked();//点击下一步按钮
    void on_confirm_alterbtn_clicked();//确认修改密码
    void innialterpass_page();//初始化修改密码界面

    //跑马灯效果
    void updateIndex();

    void on_pushButton_clicked();
};

#endif // FLIGHTWINDOW_H
