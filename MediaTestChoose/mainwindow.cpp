#include <QMediaPlayer>
#include <QFileDialog>
#include <QDir>
#include <QUrl>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "info.h"

QMediaPlayer *player = new QMediaPlayer;

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
    QString str1=QFileDialog::getOpenFileName(this, tr("Open File"),"/home",
                                                    tr("Music (*.mp3)"));
    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(str1)));
    player->setVolume(ui->spinBox->value());
    int val=20000+10000*qrand()%3;
    qint64 pos=(qint64)val;
    player->setPosition(pos);
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

void MainWindow::on_spinBox_valueChanged()
{
    player->setVolume(ui->spinBox->value());
}

void MainWindow::on_pushButton_clicked()
{
    Info * w= new Info();
    w->show();
}
