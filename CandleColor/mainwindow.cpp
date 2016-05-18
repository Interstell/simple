#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <windows.h>
#include <QMediaPlayer>
#include <QDir>
#include <QUrl>

#define min(a,b)            (((a) < (b)) ? (a) : (b))

const char* IPPORT = "192.168.56.101:5000";
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



void MainWindow::on_vol_sliderMoved(int position)
{
    player->setVolume(position);
}

void MainWindow::on_play_clicked(int emotion)
{


    QString str1;
    str1.toStdString();
    str1.fromStdString(str1.toStdString());
    int i = rand() % 3 + 1;
    switch (emotion) {
        case ANGER:
                switch (i) {
                case 1:
                    str = "C:/Files/musicForProject/angry/disturbed_-_down_with_the_sickness.mp3";
                    break;
                case 2:
                    str = "C:/Files/musicForProject/angry/rammstein_-_mein_herz_brennt.mp3";
                    break;
                case 3:
                    str = "C:/Files/musicForProject/angry/three_days_grace_-_animal_i_have_become.mp3";
                    break;
                }
                break;
        case CONTEMPT:
                switch (i) {
                case 1:
                    str = "";
                    break;
                case 2:
                    str = "";
                    break;
                case 3:
                    str = "";
                    break;
                }
                break;
        case DISGUST:
                switch (i) {
                case 1:
                    str = "C:/Files/musicForProject/disgust/ennio_morricone_-_inferno_bianco_synth.mp3";
                    break;
                case 2:
                    str = "C:/Files/musicForProject/disgust/";
                    break;
                case 3:
                    str = "C:/Files/musicForProject/disgust/";
                    break;
                }
                break;
        case FEAR:
                switch (i) {
                case 1:
                    str = "C:/Files/musicForProject/disgust/ennio_morricone_-_inferno_bianco_synth.mp3";
                    break;
                case 2:
                    str = "C:/Files/musicForProject/disgust/";
                    break;
                case 3:
                    str = "C:/Files/musicForProject/disgust/";
                    break;
                }
                break;
        case HAPPINESS:
                switch (i) {
                case 1:
                    str = "C:/Files/musicForProject/happy/mika_-_lollipop.mp3";
                    break;
                case 2:
                    str = "C:/Files/musicForProject/happy/pitbull_-_suavemente.mp3";
                    break;
                case 3:
                    str = "C:/Files/musicForProject/happy/smash_mouth_-_ all_star.mp3";
                    break;
                }
                break;
        case NEUTRAL:
                switch (i) {
                case 1:
                    str = "C:/Files/musicForProject/neutral/lewis_del_mar_-_loud(y).mp3";
                    break;
                case 2:
                    str = "C:/Files/musicForProject/neutral/the_police_-_every_breath_you_take.mp3";
                    break;
                case 3:
                    str = "C:/Files/musicForProject/neutral/welshly_arms_-_hold_on_i'm_comin.mp3";
                    break;
                }
                break;
        case SADNESS:
                switch (i) {
                case 1:
                    str = "C:/Files/musicForProject/sad/frank_sinatra_-_killing_me_softly_with_her_song.mp3";
                    break;
                case 2:
                    str = "C:/Files/musicForProject/sad/sia_-_i_m_in_here.mp3";
                    break;
                case 3:
                    str = "C:/Files/musicForProject/sad/sting_-_shape_of_my_heart.mp3";
                    break;
                }
                break;
        case SURPRISE:
                switch (i) {
                case 1:
                    str = "C:/Files/musicForProject/surprise/aerosmith_-_the_other_side.mp3";
                    break;
                case 2:
                    str = "C:/Files/musicForProject/surprise/rhcp_-_knock_me_down.mp3";
                    break;
                case 3:
                    str = "C:/Files/musicForProject/surprise/the_animals_-_boom_boom.mp3";
                    break;
                }
                break;
    }

    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators("C:\Files\Music\avener_kadebostany_-_castle_in_the_snow.mp3")));
    player->setVolume(ui->vol->value());
    player->play();
}

void MainWindow::on_pushButton_2_clicked()
{
    player->pause();
}
