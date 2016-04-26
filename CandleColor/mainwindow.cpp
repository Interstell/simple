#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <windows.h>
#define min(a,b)            (((a) < (b)) ? (a) : (b))

const char* IPPORT = "192.168.56.101:5000";



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

char* MainWindow::fromDecToHex(int Saturation, int R, int G, int B){
    static char result[20];
    char SHEX[5];
    char RHEX[5];
    char GHEX[5];
    char BHEX[5];
    _itoa(Saturation, SHEX, 16);
    _itoa(R, RHEX, 16);
    _itoa(G, GHEX, 16);
    _itoa(B, BHEX, 16);
    sprintf(result, "%02s%02s%02s%02s", SHEX, RHEX, GHEX, BHEX);
    return result;
}

void MainWindow::sendColor(void){
    char cmd[500];
    int Saturation = ui->slBrightness->value();
    int R = ui->slRed->value();
    int G = ui->slGreen->value();
    int B = ui->slBlue->value();
    MainWindow::updateLabels();
    sprintf(cmd, "curl -XPOST \"%s\" -d \"cmd:gatttool -b 80:BA:4B:11:AC:E6 --char-write -a 0x0019 -n %s\"",IPPORT, fromDecToHex(255-Saturation, R, G, B));
    WinExec(cmd, SW_HIDE);
}

void MainWindow::updateLabels(){
    int Brightness = ui->slBrightness->value();
    int Red = ui->slRed->value();
    int Green = ui->slGreen->value();
    int Blue = ui->slBlue->value();
    char buf[5];
    ui->valueBr->setText(itoa(Brightness,buf,10));
    ui->valueRed->setText(itoa(Red,buf,10));
    ui->valueGreen->setText(itoa(Green,buf,10));
    ui->valueBlue->setText(itoa(Blue,buf,10));
    char css[100];
    sprintf(css,"QLabel {border-radius: %dpx; background-color : rgba(%d,%d,%d,%d);}", min(ui->lColorView->geometry().width(),ui->lColorView->geometry().height())/2, Red,Green,Blue, Brightness);
    ui->lColorView->setStyleSheet(css);
}

void MainWindow::on_slBrightness_valueChanged(int value)
{
    if (!MainWindow::random){
        MainWindow::sendColor();
    }
}

void MainWindow::on_slRed_valueChanged(int value)
{
    if (!MainWindow::random){
        MainWindow::sendColor();
    }
}

void MainWindow::on_slGreen_valueChanged(int value)
{
    if (!MainWindow::random){
        MainWindow::sendColor();
    }
}

void MainWindow::on_slBlue_valueChanged(int value)
{

    if (!MainWindow::random){
        MainWindow::sendColor();
    }
}

void MainWindow::on_pushButton_clicked()
{
    MainWindow::random = true;

    ui->slBrightness->setValue(std::rand() % 200 + 56);
    ui->slRed->setValue(std::rand() % 256);
    ui->slGreen->setValue(std::rand() % 256);
    ui->slBlue->setValue(std::rand() % 256);
    MainWindow::sendColor();
    MainWindow::random = false;
}


void MainWindow::on_valueBr_textEdited(const QString &arg1)
{
    if (arg1.toInt() < 256 && arg1.toInt() >= 0)
        ui->slBrightness->setValue(arg1.toInt());
}

void MainWindow::on_valueRed_textEdited(const QString &arg1)
{
    if (arg1.toInt() < 256 && arg1.toInt() >= 0)
        ui->slRed->setValue(arg1.toInt());
}

void MainWindow::on_valueGreen_textEdited(const QString &arg1)
{
    if (arg1.toInt() < 256 && arg1.toInt() >= 0)
        ui->slGreen->setValue(arg1.toInt());
}

void MainWindow::on_valueBlue_textEdited(const QString &arg1)
{
    if (arg1.toInt() < 256 && arg1.toInt() >= 0)
        ui->slBlue->setValue(arg1.toInt());
}

void MainWindow::on_btnCandleEffect_clicked()
{
    MainWindow::candleEffect = (MainWindow::candleEffect)?false:true;
    ui->btnCandleEffect->setText(MainWindow::candleEffect?"CandleEffect: ON":"CandleEffect: OFF");
    if (MainWindow::candleEffect){
        sendEffect(CANDLE);
    }
    else sendEffect(NOEFFECT);
}

void MainWindow::sendEffect(effect_e effect){
    char cmd[500];
    int Saturation = 255 - ui->slBrightness->value();
    int R = ui->slRed->value();
    int G = ui->slGreen->value();
    int B = ui->slBlue->value();
    int speed;
    switch(effect){
    case CANDLE:
        speed = 1;
        break;
    case FADE:
        speed = 255 - ui->slFade->value();
        break;
    case BLINK:
        speed = 255 - ui->slBlink->value();
        break;
    case NOEFFECT:
        speed = 1;
    }

    MainWindow::updateLabels();
    sprintf(cmd, "curl -XPOST \"%s\" -d \"cmd:gatttool -b 80:BA:4B:11:AC:E6 --char-write -a 0x0017 -n %s\"",IPPORT, MainWindow::fromDecToHex(Saturation,R,G,B,effect,speed));
    WinExec(cmd, SW_HIDE);
}

char* MainWindow::fromDecToHex(int Saturation, int R, int G, int B, effect_e effect, int speed){
    static char result[20];
    char SHEX[5];
    char RHEX[5];
    char GHEX[5];
    char BHEX[5];
    int effectNum = (int)effect;
    char SPEEDHEX[5];
    _itoa(Saturation, SHEX, 16);
    _itoa(R, RHEX, 16);
    _itoa(G, GHEX, 16);
    _itoa(B, BHEX, 16);
    _itoa(speed,SPEEDHEX,16);

    sprintf(result, "%02s%02s%02s%02s%02d00%02s00",SHEX,RHEX,GHEX,BHEX,effectNum,SPEEDHEX);
    return result;
}

void MainWindow::on_slBlink_sliderReleased()
{
    sendEffect(BLINK);
}

void MainWindow::on_slFade_sliderReleased()
{
    sendEffect(FADE);
}
