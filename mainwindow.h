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

private slots:
    void on_Start1_clicked();
    void on_Reset1_clicked();
    void Timer1_valueChanged(int arg1);

private :
    Ui::MainWindow *ui;
    bool reset1 = false;
};
#endif // MAINWINDOW_H

