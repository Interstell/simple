#include <QMediaPlayer>
#include <QDir>
#include <QUrl>
#include "mainwindow.h"
#include "ui_mainwindow.h"

QMediaPlayer *player = new QMediaPlayer;
int i=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Start_clicked()
{
    QString str1=ui->lineEdit->text();
    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(str1)));
    player->setVolume(ui->spinBox->value());
    player->play();
}
void MainWindow::on_Stop_clicked()
{
    player->pause();
}


void MainWindow::on_Play_clicked()
{
    player->play();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    player->setVolume(ui->spinBox->value());
}
