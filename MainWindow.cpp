#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "flightwindow.h"
#include "adminwindow.h"
#include <QByteArray>
#include <QString>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QTextStream>
#include <stdio.h>

extern QString u_name;
extern QString u_tel;
extern QString u_IDCard;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initMyWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMyWindow() //初始化模块
{
    ui->login_radioButton->setChecked(true);
    loginWhether = false;
    ui->name_label->setVisible(false);
    ui->name_lineEdit->setVisible(false);
    ui->id_card_num_label->setVisible(false);
    ui->id_card_num_lineEdit->setVisible(false);

    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
}

void MainWindow::clearall()    //清空LineEdit功能函数
{
    ui->tel_num_lineEdit->clear();
    ui->id_card_num_lineEdit->clear();
    ui->password_lineEdit->clear();
    ui->name_lineEdit->clear();
}

void MainWindow::on_login_radioButton_clicked() //选定到登录界面
{
    clearall();
    ui->admin_login_pushButton->setVisible(true);
    if(loginWhether)
    {
        loginWhether = false;

        ui->login_register_pushButton->setText("登录");
        ui->name_label->setVisible(false);
        ui->name_lineEdit->setVisible(false);
        ui->id_card_num_label->setVisible(false);
        ui->id_card_num_lineEdit->setVisible(false);
    }

}


void MainWindow::on_register_radioButton_clicked() //选定到注册界面
{
    clearall();
    ui->admin_login_pushButton->setVisible(false);
    if(!loginWhether)
    {
        loginWhether = true;

        ui->login_register_pushButton->setText("注册");
        ui->name_label->setVisible(true);
        ui->name_lineEdit->setVisible(true);
        ui->id_card_num_label->setVisible(true);
        ui->id_card_num_lineEdit->setVisible(true);
    }
}


void MainWindow::on_show_password_pushButton_clicked()
{
    if(showPassword)
    {
        showPassword = false;
        ui->show_password_pushButton->setText("不显示密码");
        ui->password_lineEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        showPassword = true;
        ui->show_password_pushButton->setText("显示密码");
        ui->password_lineEdit->setEchoMode(QLineEdit::Password);
    }
}


void MainWindow::on_login_register_pushButton_clicked()
{
    /**普通用户登录功能**/
    if(ui->login_register_pushButton->text() == "登录")
    {
        if(ui->tel_num_lineEdit->text().isEmpty())
        {
            QMessageBox::warning(NULL, "提示", "手机号不能为空");
            ui->tel_num_lineEdit->setFocus();
        }
        else if(ui->password_lineEdit->text().isEmpty())
        {
            QMessageBox::warning(NULL, "提示", "密码不能为空");
            ui->password_lineEdit->setFocus();
        }
        else
        {
            QString str_tel = ui->tel_num_lineEdit->text();
            QString str_password = ui->password_lineEdit->text();
            QByteArray byte_password = str_password.toLatin1();
            QByteArray byte_password_md5 = QCryptographicHash::hash(byte_password,QCryptographicHash::Md5);
            QString strPwdMd5 = byte_password_md5.toHex();
            QSqlTableModel model;
            model.setTable("user_info");
            model.setFilter(tr("u_tel = '%1' and u_pwd = '%2'").arg(str_tel).arg(strPwdMd5));
            model.select();
            if(model.rowCount() == 1)
            {
    //            ac_user_tel = str_tel;
                QMessageBox::about(NULL, "提示", "登陆成功");
                u_name=model.data(model.index(0, 0)).toString();
                u_tel=str_tel;
                u_IDCard=model.data(model.index(0, 1)).toString();

                this->close();
                FlightWindow *fw = new FlightWindow;
                fw->setWindowTitle("用户机票管理");
                fw->show();
            }
            else
            {
                QMessageBox::warning(NULL, "提示", "账户或密码错误，请重新输入!");
                ui->tel_num_lineEdit->clear();
                ui->password_lineEdit->clear();
                ui->tel_num_lineEdit->setFocus();
            }
        }

    }

    /*管理员登录功能*/
    if(ui->login_register_pushButton->text() == "管理员登录")
    {
        if(ui->tel_num_lineEdit->text().isEmpty())
        {
            QMessageBox::warning(NULL, "提示", "管理员账号不能为空");
            ui->tel_num_lineEdit->setFocus();
        }
        else if(ui->password_lineEdit->text().isEmpty())
        {
            QMessageBox::warning(NULL, "提示", "密码不能为空");
            ui->password_lineEdit->setFocus();
        }
        else
        {
            QString str_tel = ui->tel_num_lineEdit->text();
            QString str_password = ui->password_lineEdit->text();
//            QByteArray byte_password = str_password.toLatin1();
//            QByteArray byte_password_md5 = QCryptographicHash::hash(byte_password,QCryptographicHash::Md5);
//            QString strPwdMd5 = byte_password_md5.toHex();
            QSqlTableModel model;
            model.setTable("admin_info");
            model.setFilter(tr("ad_id = '%1' and ad_pwd = '%2'").arg(str_tel).arg(str_password));
            model.select();
            if(model.rowCount() == 1)
            {

                QMessageBox::about(NULL, "提示", "登陆成功");
                this->close();
                AdminWindow *aw = new AdminWindow;
                aw->setWindowTitle("管理员机票管理");
                aw->show();
            }
            else
            {
                QMessageBox::warning(NULL, "提示", "账户或密码错误，请重新输入!");
                ui->tel_num_lineEdit->clear();
                ui->password_lineEdit->clear();
                ui->tel_num_lineEdit->setFocus();
            }
        }

    }
    /*普通用户注册功能*/
    if(ui->login_register_pushButton->text() == "注册")
    {
        if(ui->name_lineEdit->text().isEmpty() || ui->id_card_num_lineEdit->text().isEmpty() || ui->tel_num_lineEdit->text().isEmpty() || ui->password_lineEdit->text().isEmpty())
        {
            QMessageBox::warning(NULL, "提示", "请输入完整信息");
        }
        else
        {
            QTextStream cout(stdout,  QIODevice::WriteOnly);
            QByteArray byte_name = ui->name_lineEdit->text().toUtf8();
            QByteArray byte_id = ui->id_card_num_lineEdit->text().toUtf8();
            QByteArray byte_tel = ui->tel_num_lineEdit->text().toUtf8();
            QString str_password = ui->password_lineEdit->text();
            QByteArray byte_password = str_password.toLatin1();
            QByteArray byte_password_md5 = QCryptographicHash::hash(byte_password,QCryptographicHash::Md5);
            QString strPwdMd5 = byte_password_md5.toHex();
            QByteArray b_password_md5 = strPwdMd5.toLatin1();
            char *c_name = byte_name.data();
            char *c_id = byte_id.data();
            char *c_tel = byte_tel.data();
            char *c_password_md5 = b_password_md5.data();

            cout<<c_password_md5;
            QString str = QString("insert into user_info(u_name, u_IDCard, u_tel, u_pwd)values('%1','%2','%3','%4')").arg(c_name).arg(c_id).arg(c_tel).arg(c_password_md5);

            QSqlTableModel model;
            model.setTable("user_info");
            model.setFilter(tr("u_tel = '%1'").arg(ui->tel_num_lineEdit->text()));
            model.select();
            if(model.rowCount() == 1)
            {
                QMessageBox::warning(NULL, "提示", "手机号已注册");
                ui->tel_num_lineEdit->clear();
                ui->tel_num_lineEdit->setFocus();
            }
            else
            {
                QSqlQuery query;
                if(query.exec(str))
                {
                    QMessageBox::about(NULL, "提示", "注册成功");
                    ui->password_lineEdit->clear();
                    ui->name_lineEdit->clear();
                    ui->id_card_num_lineEdit->clear();

                    ui->login_radioButton->setChecked(true);

                    loginWhether = false;

                    ui->login_register_pushButton->setText("登录");
                    ui->name_label->setVisible(false);
                    ui->name_lineEdit->setVisible(false);
                    ui->id_card_num_label->setVisible(false);
                    ui->id_card_num_lineEdit->setVisible(false);
                }
                else
                {
                    QMessageBox::warning(NULL, "警告", "注册失败，请检查信息！");
                }
            }
        }
    }




}


void MainWindow::on_admin_login_pushButton_clicked()
{
    clearall();
    if(ui->admin_login_pushButton->text() == "管理员登录")
    {
        ui->login_radioButton->setVisible(false);
        ui->register_radioButton->setVisible(false);
        ui->login_register_pushButton->setText("管理员登录");
        ui->admin_login_pushButton->setText("用户登录");
        ui->tel_num_label->setText("管理员账号:");
        ui->password_label->setText("管理员密码:");
        ui->name_label->setVisible(false);
        ui->name_lineEdit->setVisible(false);
        ui->id_card_num_label->setVisible(false);
        ui->id_card_num_lineEdit->setVisible(false);
    }
    else if(ui->admin_login_pushButton->text() == "用户登录")
    {
        ui->login_radioButton->setVisible(true);
        ui->register_radioButton->setVisible(true);
        ui->login_register_pushButton->setText("登录");
        ui->admin_login_pushButton->setText("管理员登录");
        ui->tel_num_label->setText("手机号码:");
        ui->password_label->setText("密码:");
        ui->name_label->setVisible(false);
        ui->name_lineEdit->setVisible(false);
        ui->id_card_num_label->setVisible(false);
        ui->id_card_num_lineEdit->setVisible(false);
    }
}

