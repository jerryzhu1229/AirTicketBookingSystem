#ifndef ALTERDIALOG_H
#define ALTERDIALOG_H

#include <QDialog>

namespace Ui {
class alterdialog;
}

class alterdialog : public QDialog
{
    Q_OBJECT

public:
    explicit alterdialog(QWidget *parent = nullptr);
    ~alterdialog();

private:
    Ui::alterdialog *ui;
    bool is_alter=false;//判断是否已经改签过

    QString flight_code;//航班编号
    QString s_date;//出发日期
    QString s_time;//出发时间
    QString f_port;//出发机场
    QString t_port;//目的机场

    QString new_flight_code;//获得新的航班编号
    QString new_seat_code;//获得新的座位编号
    int new_order_total;//获得新的票价
private slots:
    void initAlterDialog();//初始化对话框
    void on_pushButton_clicked();//确认改签
    void get_row();//判断选中了哪一行
};

#endif // ALTERDIALOG_H
