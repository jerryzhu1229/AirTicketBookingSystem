#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool loginWhether = true;
    bool showPassword = true;
    bool adminLogin = true;

private slots:
    void clearall();
    void initMyWindow();
    void on_login_radioButton_clicked();

    void on_register_radioButton_clicked();

    void on_show_password_pushButton_clicked();

    void on_login_register_pushButton_clicked();

    void on_admin_login_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
