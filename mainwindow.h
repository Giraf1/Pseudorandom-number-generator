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
    void on_checkBox_stateChanged(int arg1);

    void on_lineEdit_min_textChanged(const QString &arg1);

    void on_pushButton_gen_clicked();

    void on_radioButton_lin_clicked(bool checked);

    void set_visibility_group_1(bool a);

    void on_radioButton_system_clicked(bool checked);

    QString check_input(QString input);

    void on_lineEdit_max_textChanged(const QString &arg1);

    void on_lineEdit_amount_textChanged(const QString &arg1);

    void on_lineEdit_seed_textChanged(const QString &arg1);

    void on_lineEdit_a_textChanged(const QString &arg1);

    void on_lineEdit_c_textChanged(const QString &arg1);

    unsigned long int myGenerator(int max,int min);

    int qGenerator(int max, int min);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
