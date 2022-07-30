#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <fstream>
#include <cstring>
#include "ctype.h"

#include <iostream>

#include <QString>
#include <QObject>
#include <QRandomGenerator>
#include <QDebug>
#include <QLabel>
#include <QAudio>
#include <QtMultimedia>
#include <QMediaPlayer>
#include <QRegularExpression>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QButtonGroup>
//#include <QWebCam>


QMediaPlayer* player = new QMediaPlayer();
QAudioOutput* output = new QAudioOutput();


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //if (!QWebCam::Detect(Marchenko)) exit(0);

    QButtonGroup *group_button_out = new QButtonGroup;
    QButtonGroup *group_button_gen = new QButtonGroup;

    group_button_out->addButton(ui->radioButton_file);
    group_button_out->addButton(ui->radioButton_window);

    group_button_gen->addButton(ui->radioButton_system);
    group_button_gen->addButton(ui->radioButton_lin);

    ui->radioButton_window->setChecked(true);
    ui->radioButton_system->setChecked(true);

    MainWindow::set_visibility_group_1(false);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    delete output;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->lineEdit_a->setEnabled(true);
        ui->lineEdit_c->setEnabled(true);
        ui->lineEdit_seed->setEnabled(true);
    }
    else {
        ui->lineEdit_a->setEnabled(false);
        ui->lineEdit_c->setEnabled(false);
        ui->lineEdit_seed->setEnabled(false);
    }
}




void MainWindow::on_pushButton_gen_clicked()
{
    ui->textEdit_output->clear();
    if(ui->lineEdit_min->text().size() == 0) QMessageBox::warning(this, tr("Миссия успешно провалена!"), tr("Вы ввели NULL в строчку ввода минимального значения, подумайте ещё раз."));
    else if(ui->lineEdit_max->text().size() == 0) QMessageBox::warning(this, tr("Миссия успешно провалена!"), tr("Вы ввели NULL в строчку ввода максимального значения, подумайте ещё раз."));
    else if(ui->lineEdit_amount->text().size() == 0) QMessageBox::warning(this, tr("Миссия успешно провалена!"), tr("Вы ввели NULL в строчку ввода количества значений, подумайте ещё раз."));
    else if(ui->lineEdit_max->text() == ui->lineEdit_min->text()) QMessageBox::warning(this, tr("Миссия успешно провалена!"), tr("Максимальное и минимальное значение не могут быть равными, подумайте ещё раз."));
    else if(ui->lineEdit_max->text().toInt() < ui->lineEdit_min->text().toInt()) QMessageBox::warning(this, tr("Миссия успешно провалена!"), tr("Максимальное значение не может быть меньше минимального, подумайте ещё раз."));
    else if(ui->lineEdit_amount->text().toInt() == 0) QMessageBox::warning(this, tr("Миссия успешно провалена!"), tr("Количество выводимых чисел не может быть нулевым, подумайте ещё раз."));
    else {
        ui->progressBar->setMaximum(ui->lineEdit_amount->text().toInt());
        if(ui->radioButton_system->isChecked()) {
            if(ui->radioButton_window->isChecked()) {
                for (int i = 0; i < ui->lineEdit_amount->text().toInt(); i++) {
                    ui->progressBar->setValue(i+1);
                    ui->textEdit_output->append(QString::number(MainWindow::qGenerator(ui->lineEdit_max->text().toInt(), ui->lineEdit_min->text().toInt())));
                }

            }
            else if(ui->radioButton_file->isChecked()) {
                QFile oFile(QDir::currentPath() + "/output.txt");
                oFile.open(QIODevice::WriteOnly);
                QTextStream out(&oFile);
                out.setEncoding(QStringConverter::Utf8);
                for (int i = 0; i < ui->lineEdit_amount->text().toInt(); i++) {
                    ui->progressBar->setValue(i+1);
                    out << QString::number(MainWindow::qGenerator(ui->lineEdit_max->text().toInt(), ui->lineEdit_min->text().toInt())) << "\n";
                }
                oFile.close();
                QMessageBox inf;
                inf.setText("Текстовый документ сохранён по директории хранения этого приложения, Путь: \n" + QDir::currentPath());
                inf.exec();
            }
        }
        else if (ui->radioButton_lin->isChecked()) {
            if(ui->radioButton_window->isChecked()) {
                for (int i = 0; i < ui->lineEdit_amount->text().toInt(); i++) {
                    ui->progressBar->setValue(i+1);
                    ui->textEdit_output->append(QString::number(MainWindow::myGenerator(ui->lineEdit_max->text().toInt(), ui->lineEdit_min->text().toInt())));
                }
            }
            else if(ui->radioButton_file->isChecked()) {
                QFile oFile(QDir::currentPath() + "/output.txt");
                oFile.open(QIODevice::WriteOnly);
                QTextStream out(&oFile);
                out.setEncoding(QStringConverter::Utf8);
                for (int i = 0; i < ui->lineEdit_amount->text().toInt(); i++) {
                    ui->progressBar->setValue(i+1);
                    out << QString::number(MainWindow::myGenerator(ui->lineEdit_max->text().toInt(), ui->lineEdit_min->text().toInt())) << "\n";
                }
                oFile.close();
                QMessageBox inf;
                inf.setText("Текстовый документ сохранён по директории хранения этого приложения, Путь: \n" + QDir::currentPath());
                inf.exec();
            }
        }
    }
}


void MainWindow::set_visibility_group_1(bool a)
{
    ui->label_1_1->setVisible(a);
    ui->label_1_2->setVisible(a);
    ui->label_1_4->setVisible(a);
    ui->label_1_5->setVisible(a);
    ui->checkBox->setVisible(a);
    ui->lineEdit_a->setVisible(a);
    ui->lineEdit_c->setVisible(a);
    ui->lineEdit_seed->setVisible(a);
}

QRegularExpression re("^\\d+$"); // только цифры
int max_input_size = 7;


QString MainWindow::check_input(QString input)
{
    QRegularExpressionMatch check = re.match(input);
    if (!check.hasMatch()) {
        input.resize(input.size()-1);
        player->setSource(QUrl::fromLocalFile("C:/User/Girafi/Documents/media/WError.wav"));
        player->setAudioOutput(output);
        output->setVolume(50);
        player->play();
    }
    if (input.size() > max_input_size) input.resize((input.size() - 1));
    return(input);
}

unsigned long int temp(-1);
unsigned long int previos_random_number = 1;

unsigned long int MainWindow::myGenerator(int max,int min)
{
    while(true) {
        unsigned long int output = (((ui->lineEdit_a->text().toInt()*previos_random_number+ui->lineEdit_c->text().toInt())/256)%4194304);
        previos_random_number = output;
        if ((output%((max+1)-min)+min) != temp) return (unsigned int) (temp = (output%((max+1)-min)+min));
    }
}


int MainWindow::qGenerator(int max, int min)
{
    while(true) {
        unsigned long int output_n = QRandomGenerator::system()->generate()%((max + 1) - min) + min;
        if (output_n != temp) return (unsigned int) (temp = output_n);
    }
}



void MainWindow::on_radioButton_lin_clicked(bool checked)
{
    MainWindow::set_visibility_group_1(checked);
}

void MainWindow::on_radioButton_system_clicked(bool checked)
{
    MainWindow::set_visibility_group_1(!checked);
}


void MainWindow::on_lineEdit_min_textChanged   (const QString &arg1){ui->lineEdit_min->setText(MainWindow::check_input(arg1));}
void MainWindow::on_lineEdit_max_textChanged   (const QString &arg1){ui->lineEdit_max->setText(MainWindow::check_input(arg1));}
void MainWindow::on_lineEdit_amount_textChanged(const QString &arg1){ui->lineEdit_amount->setText(MainWindow::check_input(arg1));}
void MainWindow::on_lineEdit_seed_textChanged  (const QString &arg1){ui->lineEdit_seed->setText(MainWindow::check_input(arg1));}
void MainWindow::on_lineEdit_a_textChanged     (const QString &arg1){ui->lineEdit_a->setText(MainWindow::check_input(arg1));}
void MainWindow::on_lineEdit_c_textChanged     (const QString &arg1){ui->lineEdit_c->setText(MainWindow::check_input(arg1));}
