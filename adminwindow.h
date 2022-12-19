#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();


private slots:
    void on_query_way_comboBox_currentTextChanged(const QString &arg1);

    void initAdminWindow();

    void on_flightQueryBtn_2_clicked();

    void clear_flight_info_table();

    void clear_passenage_info_table();

    void clear_order_info_table();

    void flight_date_que();

    void flight_num_que();

    void passenage_tel_que();

    void order_que();

    void show_all_passenage_info();

    void show_all_flight_info();

    void show_all_order_info();

    void on_flight_info_veritfyBtn_3_clicked();

    void on_flight_info_deleteBtn_4_clicked();

    void on_flight_Btn_clicked();

    void on_passenger_Btn_clicked();

    void on_order_Btn_clicked();

    void on_pasnger_info_que_pushButton_clicked();

    void on_passenger_info_veritfyBtn_4_clicked();

    void on_flight_info_deleteBtn_5_clicked();

    void on_order_que_pushButton_clicked();

    void on_order_info_veritfyBtn_5_clicked();

    void on_order_info_deleteBtn_6_clicked();

    void on_logout_pushButton_clicked();

private:
    Ui::AdminWindow *ui;
    QString flight_code; //航班编号
    QString plane_code; //机型
    QString ecomy_price; //最低价
    QString start_date; //出发日期
    QString start_time; //出发时间
    QString arrive_time; //到达时间
    QString from_city; //出发城市
    QString to_city; //到底城市
    QString from_port; //出发机场
    QString to_port; //到达机场
    QString flight_status; //航班状态

    QByteArray byte_verifty_flight_code; //需要修改的航班编号
    QByteArray byte_verifty_plane_code; //需要修改的机型
    QByteArray byte_verifty_ecomy_price; //需要修改的最低价
    QByteArray byte_verifty_start_date; //需要修改的出发日期
    QByteArray byte_verifty_start_time; //需要修改的出发时间
    QByteArray byte_verifty_arrive_time; //需要修改的到达时间
    QByteArray byte_verifty_from_city; //需要修改的出发城市
    QByteArray byte_verifty_to_city; //需要修改的到底城市
    QByteArray byte_verifty_from_port; //需要修改的出发机场
    QByteArray byte_verifty_to_port; //需要修改的到达机场
    QByteArray byte_verifty_flight_status; //需要修改的航班状态

    char* verifty_flight_code; //需要修改的航班编号
    char* verifty_plane_code; //需要修改的机型
    char* verifty_ecomy_price; //需要修改的最低价
    char* verifty_start_date; //需要修改的出发日期
    char* verifty_start_time; //需要修改的出发时间
    char* verifty_arrive_time; //需要修改的到达时间
    char* verifty_from_city; //需要修改的出发城市
    char* verifty_to_city; //需要修改的到底城市
    char* verifty_from_port; //需要修改的出发机场
    char* verifty_to_port; //需要修改的到达机场
    char* verifty_flight_status; //需要修改的航班状态

    QString user_name; //旅客姓名
    QString user_code; //旅客身份证
    QString user_tel; //旅客电话号
    QString user_pwd; //旅客密码

    QByteArray byte_verifty_user_name; //需要修改的旅客姓名
    QByteArray byte_verifty_user_code; //需要修改的旅客身份证
    QByteArray byte_verifty_user_tel; //需要修改的旅客电话号
    QByteArray byte_verifty_user_pwd; //需要修改的旅客密码

    char* verifty_user_name; //需要修改的旅客姓名
    char* verifty_user_code; //需要修改的旅客身份证
    char* verifty_user_tel; //需要修改的旅客电话号
    char* verifty_user_pwd; //需要修改的旅客密码


    QString order_code; //订单编号
//    QString flight_code; //机型
    QString seat_code; //座位编号
    QString booku_tel; //预定者电话
    QString passger_tel; //旅客电话
    QString order_total; //小计
    QString order_status; //订单状态
    QString create_time; //创建时间

    QByteArray byte_verifty_order_code; //需要修改的订单编号
    QByteArray byte_verifty_seat_code; //需要修改的座位编号
    QByteArray byte_verifty_booku_tel; //需要修改的预定者电话
    QByteArray byte_verifty_passger_tel; //需要修改的旅客电话
//    QByteArray byte_verifty_order_total; //需要修改的小计
//    QByteArray byte_verifty_order_status; //需要修改的订单状态
    QByteArray byte_verifty_create_time; //需要修改的创建时间

    char* verifty_order_code; //需要修改的订单编号
    char* verifty_seat_code; //需要修改的座位编号
    char* verifty_booku_tel; //需要修改的预定者电话
    char* verifty_passger_tel; //需要修改的旅客电话
//    char* verifty_order_total; //需要修改的小计
//    char* verifty_order_status; //需要修改的订单状态
    char* verifty_create_time; //需要修改的创建时间

};

#endif // ADMINWINDOW_H
