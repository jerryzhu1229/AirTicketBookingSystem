#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include <QDialog>

namespace Ui {
class bookDialog;
}

class bookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit bookDialog(QWidget *parent = nullptr);
    ~bookDialog();

private:
    Ui::bookDialog *ui;

    int ecom_price;//经济舱价格
    int bus_price;//商务舱价格
    int best_price;//头等舱价格
    int ecom_num;//经济舱库存数量
    int bus_num;//商务舱库存数量
    int best_num;//头等舱库存数量

    QString p_name;//乘客姓名
    QString p_tel;//乘客电话
    QString p_IDCard;//乘客身份证

    QString order_code;//订单编号
    QString seat_code;//座位编号
    QString booku_tel;//预定者电话
    QString passger_tel;//乘客电话
    QString passger_IDCard;//乘客身份证
    int order_total;//机票票价
    QString order_status;//订单状态
    QString create_time;//支付时间
private slots:
    void initBookDialog();//初始化对话框
    void on_bookpushBtn_clicked();  //预定
    void on_cancelBtn_clicked();   //退出

};

#endif // BOOKDIALOG_H
