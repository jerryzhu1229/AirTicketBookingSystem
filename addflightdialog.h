#ifndef ADDFLIGHTDIALOG_H
#define ADDFLIGHTDIALOG_H

#include <QDialog>

namespace Ui {
class addflightdialog;
}

class addflightdialog : public QDialog
{
    Q_OBJECT

public:
    explicit addflightdialog(QWidget *parent = nullptr);
    ~addflightdialog();

private slots:
    void on_pushButton_clicked();

    void OnSelectChanged_fromcity();

    void OnSelectChanged_tocity();
private:
    Ui::addflightdialog *ui;

    void initPlane();

    void initCity();

    void initaddflightdialog();





};

#endif // ADDFLIGHTDIALOG_H
