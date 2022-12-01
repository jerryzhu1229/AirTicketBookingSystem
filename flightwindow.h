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
    void clear_flightTabWidget();//清理航班表格信息
    void set_menuBtnStyle();//设置菜单按钮样式

    void initorder_page();//初始化订单界面
    void on_searchpushBtn_clicked();//查询全部订单
};

#endif // FLIGHTWINDOW_H
