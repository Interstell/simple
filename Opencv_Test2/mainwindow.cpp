#define _CRT_SECURE_NO_WARNINGS
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cjson.h"
#include <QBitmap>
#include <QRegion>
#include <Windows.h>

const char* IPPORT = "192.168.56.101:5000";
#define NEUTRAL_CORRECTION_PERCENT 0.85

struct EMOTION{
    char type[20];
    double value;
    enum emoEnum type_e;
};

enum emoEnum{
    ANGER = 1,
    CONTEMPT,
    DISGUST,
    FEAR,
    HAPPINESS,
    NEUTRAL,
    SADNESS,
    SURPRISE
};

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

void MainWindow::on_startBtn_clicked()
{
    using namespace cv;
    using namespace std;
    ui->emotionBtn->setEnabled(true);
    player = new QMediaPlayer;
    VideoCapture cap;
    cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,960);
    cap.open(0);
    QPixmap qimg;

    namedWindow("EmotionAnalyzer",CV_WINDOW_NORMAL | CV_WINDOW_FREERATIO | CV_GUI_NORMAL);
    resizeWindow("EmotionAnalyzer",1,1);
    moveWindow("EmotionAnalyzer",2000,2000);
    MainWindow::toCapture = true;

    // Load cascate classifier placed in sulution folder
    CascadeClassifier detector;
    string cascadeName = "haarcascade_frontalface_alt.xml";
    bool loaded = detector.load(cascadeName);

    // Parameters of detectMultiscale Cascade Classifier
    int groundThreshold = 2;
    double scaleStep = 1.1;
    Size minimalObjectSize(80, 80);
    Size maximalObjectSize(350, 350);

    // Vector of returned faces
    vector<Rect> found;
    GetLocalTime(&timeForEmo);
    while(1){
        //Mat img;
        cap >> MainWindow::img;

        // Convert input to greyscale
        Mat image_grey;
        cvtColor(MainWindow::img, image_grey, CV_BGR2GRAY);

        // why not
        found.clear();

        // Detect faces
        detector.detectMultiScale(image_grey, found, scaleStep, groundThreshold, 0 | 2, minimalObjectSize, maximalObjectSize);

        int largestFace = 0;
        if (found.size() > 1)
        for (int i = 1; i < found.size(); i++)
            if (found[i].area() > found[0].area()) largestFace = i;

        // Draw the results into mat retrieved from webcam
        if (found.size() > 0) {
            for (int i = 0; i < found.size(); i++) {
                 //rectangle(MainWindow::img, found[i].br(), found[i].tl(), Scalar(0, 0, (i == largestFace) ? 255 : 0), 2, 4, 0);
                rectangle(MainWindow::img, found[i].br(), found[i].tl(), (i == largestFace)?Scalar(0,255,0):Scalar(255,255,255), 2, 4, 0);
            }
        }

        qimg = QPixmap::fromImage(QImage((unsigned char*) MainWindow::img.data,
                                          MainWindow::img.cols,
                                          MainWindow::img.rows,
                                          QImage::Format_RGB888).rgbSwapped());
        qimg = qimg.scaledToWidth(ui->label->geometry().width());
        ui->label->setPixmap(qimg);
        ui->label->show();

        waitKey(20);

        if (!MainWindow::isVisible() || MainWindow::toCapture == false){ // stop capturing by pressing ESC
            destroyWindow("EmotionAnalyzer");
            break; //exit
        }

        SYSTEMTIME check;
        GetLocalTime(&check);
        long diff = check.wHour*60*60*1000 + check.wMinute*60*1000 + check.wSecond*1000 + check.wMilliseconds - (timeForEmo.wHour*60*60*1000 + timeForEmo.wMinute*60*1000 + timeForEmo.wSecond*1000 + timeForEmo.wMilliseconds);
        if (diff > EMOWAITTIME){
            ui->emotionBtn->setEnabled(true);
            ui->emotionBtn->click();
        }
        img.release();
    }

}


void MainWindow::on_stopBtn_clicked()
{
    MainWindow::toCapture = false;
    ui->emotionBtn->setEnabled(false);
    ui->label->setVisible(false);
    delete player;
    player = NULL;
    ui->musicTrackNameLabel->setText("");
}


int compareEmos(const void* a, const void * b){
    double dif = ((struct EMOTION*)b)->value - ((struct EMOTION*)a)-> value;
    if (dif > 0) return 1;
    else if (dif < 0) return -1;
    else return 0;
}


double* MainWindow::emotions_Analyze(char* link){
    char cmd[600];
    char exportFileName[400];
    sprintf(exportFileName, "emotions.json");
    sprintf(cmd, "curl.exe -o %s -XPOST https://api.projectoxford.ai/emotion/v1.0/recognize?subscription-key=349f1c7ff1e642498d333cb2fe5fab25 -k --data-binary @%s -H \"Content-Type:application/octet-stream\"", exportFileName, link);
    WinExec(cmd,SW_HIDE);

    FILE* fJSON = fopen(exportFileName, "r");

    char jsonContent[1000];
    fread(jsonContent,sizeof(char),1000,fJSON);
    cJSON* root = cJSON_Parse(jsonContent);
    int size = cJSON_GetArraySize(root);
    if (size == 0){//no face detected
        return NULL;
    }
    if (cJSON_GetObjectItem(root, "error") != NULL || cJSON_GetObjectItem(root, "message") != NULL ){
        return NULL;
    }

    int biggestFace = 0;
    int biggestArea = 0;

    for (int i = 0; i < cJSON_GetArraySize(root); i++){
        cJSON* data = cJSON_GetArrayItem(root, i);
        cJSON * faceRectangle = cJSON_GetObjectItem(data, "faceRectangle");
        cJSON * width = cJSON_GetObjectItem(faceRectangle, "width");
        cJSON * height = cJSON_GetObjectItem(faceRectangle, "height");
        int area = width->valueint * height->valueint;
        if (area > biggestArea) {
            biggestArea = area;
            biggestFace = i;
        }
    }

    cJSON* data = cJSON_GetArrayItem(root, biggestFace);
    cJSON* scores = cJSON_GetObjectItem(data, "scores");
    cJSON* anger = cJSON_GetObjectItem(scores, "anger");
    cJSON* contempt = cJSON_GetObjectItem(scores, "contempt");
    cJSON* disgust = cJSON_GetObjectItem(scores, "disgust");
    cJSON* fear = cJSON_GetObjectItem(scores, "fear");
    cJSON* happiness = cJSON_GetObjectItem(scores, "happiness");
    cJSON* neutral = cJSON_GetObjectItem(scores, "neutral");
    cJSON* sadness = cJSON_GetObjectItem(scores, "sadness");
    cJSON* surprise = cJSON_GetObjectItem(scores, "surprise");

    double* emotionsArray = new double[8];
    emotionsArray[0] = anger->valuedouble;
    emotionsArray[1] = contempt->valuedouble;
    emotionsArray[2] = disgust->valuedouble;
    emotionsArray[3] = fear->valuedouble;
    emotionsArray[4] = happiness->valuedouble;
    emotionsArray[5] = neutral->valuedouble;
    emotionsArray[6] = sadness->valuedouble;
    emotionsArray[7] = surprise->valuedouble;

    fclose(fJSON);

    return emotionsArray;
}

char* MainWindow::fromDecToHex(int Saturation, int R, int G, int B){
    static char result[10];
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

void MainWindow::sendColor(int Saturation, int R, int G, int B){
    char cmd[500];
    sprintf(cmd, "curl2 -XPOST \"%s\" -d \"cmd:gatttool -b 80:BA:4B:11:AC:E6 --char-write -a 0x0017 -n %s0400ff00\"",IPPORT,MainWindow::fromDecToHex(Saturation, R, G, B));
    WinExec(cmd, SW_HIDE);
}

void MainWindow::on_emotionBtn_clicked()
{
    ui->emotionBtn->setEnabled(false);
    GetLocalTime(&timeForEmo);
    MainWindow::emotionButtonPressed = true;
    imwrite("img.jpg", MainWindow::img);
    double* emotions = MainWindow::emotions_Analyze("img.jpg");
    if (!emotions){
        ui->emotionBtn->setEnabled(true);
        return; //NULL
    }
    char res[400];
    sprintf(res, "Anger: %f\nContempt: %f\nDisgust: %f\nFear: %f\nHappiness: %f\nNeutral: %f\nSadness: %f\nSurprise: %f\n", emotions[0], emotions[1],emotions[2],emotions[3],emotions[4],emotions[5], emotions[6], emotions[7]);
    //ui->emoLabel->setText(res);

    struct EMOTION emos[8]={
        "Anger",emotions[0],ANGER,
        "Contempt",emotions[1],CONTEMPT,
        "Disgust",emotions[2],DISGUST,
        "Fear",emotions[3],FEAR,
        "Happiness",emotions[4],HAPPINESS,
        "Neutral",emotions[5],NEUTRAL,
        "Sadness",emotions[6],SADNESS,
        "Surprise",emotions[7],SURPRISE};
    std::qsort((void*)emos,8,sizeof(struct EMOTION),compareEmos);
    char top[200];
    sprintf(top,"Top 3 emotions:\n%9s : %2.1f%%\n%9s : %2.1f%%\n%9s : %2.1f%%\n",emos[0].type, emos[0].value*100, emos[1].type, emos[1].value*100, emos[2].type, emos[3].value*100);
    ui->topEmoLabel->setText(top);
    int maxEmoIndex = 0;
    if (strcmp(emos[0].type,"Neutral") == 0 && emos[0].value < NEUTRAL_CORRECTION_PERCENT)
        maxEmoIndex = 1;

    int R = 0,G = 0,B = 0;

    switch (emos[maxEmoIndex].type_e){
    case ANGER: //anger
        //230,37,50 //near purple
        R = 230;
        G = 37;
        B = 50;
        break;
    case CONTEMPT: //contempt
        //210,105,30 chocolate
        R = 210;
        G = 105;
        B = 30;
        break;
    case DISGUST: //disgust
        //145,102,173 //violet
        R = 145;
        G = 102;
        B = 173;
        break;
    case FEAR: //fear
        //4,139,70 green
        R = 4;
        G = 139;
        B = 70;
        break;
    case HAPPINESS: //happiness
        //255,170,0 orangered
        R = 255;
        G = 170;
        B = 0;
        break;
    case NEUTRAL: //neutral
        //206,223,233 gray with blue
        R = 206;
        G = 223;
        B = 233;
        break;
    case SADNESS: //sadness
        //37,128,186 deep blue
        R = 37;
        G = 128;
        B = 186;
        break;
    case SURPRISE: //surprise
       //241,205,21 yellow
        R = 241;
        G = 205;
        B = 21;
        break;
    }
    if (currentEmotion != emos[maxEmoIndex].type_e){
        music_PlayByEmo(emos[maxEmoIndex].type_e);
    }
    currentEmotion = emos[maxEmoIndex].type_e;
    MainWindow::sendColor(0,R,G,B);
    char cssForLabel[100];
    sprintf(cssForLabel,"QLabel {border-radius: %dpx; background-color : rgba(%d,%d,%d,255);}", min(ui->emoLabel->geometry().width(),ui->emoLabel->geometry().height())/2, R,G,B);
    ui->emoLabel->setStyleSheet(cssForLabel);

    delete[] emotions;
    //ui->emotionBtn->setEnabled(true);
}

void MainWindow::music_PlayByEmo(enum emoEnum emo){

    std::string str1;
    //str1.toStdString();
    QString trackName;
    int i = rand() % 3 + 1;
    switch (emo) {
        case ANGER:
                switch (i) {
                case 1:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\angry\\disturbed_-_down_with_the_sickness.mp3";
                    trackName = "Disturbed - Down with the sickness";
                    break;
                case 2:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\angry\\rammstein_-_mein_herz_brennt.mp3";
                    trackName = "Rammstein - Mein Herz Brennt";
                    break;
                case 3:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\angry\\three_days_grace_-_animal_i_have_become.mp3";
                    trackName = "Three Days Grace - Animal I have_become";
                    break;
                }
                break;
        case CONTEMPT:
                switch (i) {
                case 1:
                    str1 = "";
                    trackName = "Not yet added";
                    break;
                case 2:
                    str1 = "";
                    trackName = "Not yet added";
                    break;
                case 3:
                    str1 = "";
                    trackName = "Not yet added";
                    break;
                }
                break;
        case DISGUST:
                switch (i) {
                case 1:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\disgust\\ennio_morricone_-_inferno_bianco_synth.mp3";
                    trackName = "Ennio Morricone - Inferno Bianco Synth";
                    break;
                case 2:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\disgust\\";
                    trackName = "Not yet added";
                    break;
                case 3:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\disgust\\";
                    trackName = "Not yet added";
                    break;
                }
                break;
        case FEAR:
                switch (i) {
                case 1:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\fear\\ennio_morricone_-_l'ultima_diligenza_di_red_rock.mp3";
                    trackName = "Ennio Morricone - L'ultima diligenza";
                    break;
                case 2:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\fear\\ennio_morricone_-_overture.mp3";
                    trackName = "Ennio Morricone - Overture";
                    break;
                case 3:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\fear\\marco_margna_-_stormy_ocean.mp3";
                    trackName = "Ennio Morricone - Stormy Ocean";
                    break;
                }
                break;
        case HAPPINESS:
                switch (i) {
                case 1:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\happy\\mika_-_lollipop.mp3";
                    trackName = "Mika - Lollipop";
                    break;
                case 2:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\happy\\pitbull_-_suavemente.mp3";
                    trackName = "Pitbull - Suavemente";
                    break;
                case 3:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\happy\\smash_mouth_-_ all_star.mp3";
                    trackName = "Smash Mouth - All Star";
                    break;
                }
                break;
        case NEUTRAL:
                switch (i) {
                case 1:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\neutral\\lewis_del_mar_-_loud(y).mp3";
                    trackName = "Lewis Del Mar - Loud";
                    break;
                case 2:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\neutral\\the_police_-_every_breath_you_take.mp3";
                    trackName = "The Police - Every Breath You Take";
                    break;
                case 3:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\neutral\\welshly_arms_-_hold_on_i'm_comin.mp3";
                    trackName = "Welshly Arms - Hold On I'm comin'";
                    break;
                }
                break;
        case SADNESS:
                switch (i) {
                case 1:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\sad\\frank_sinatra_-_killing_me_softly_with_her_song.mp3";
                    trackName = "Frank Sinatra - Killing me softly with her song";
                    break;
                case 2:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\sad\\sia_-_i_m_in_here.mp3";
                    trackName = "Sia - I'm in here";
                    break;
                case 3:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\sad\\sting_-_shape_of_my_heart.mp3";
                    trackName = "Sting - Shape of my heart";
                    break;
                }
                break;
        case SURPRISE:
                switch (i) {
                case 1:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\surprise\\aerosmith_-_the_other_side.mp3";
                    trackName = "Aerosmith - The Other Side";
                    break;
                case 2:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\surprise\\rhcp_-_knock_me_down.mp3";
                    trackName = "RHCP - Knock Me Down";
                    break;
                case 3:
                    str1 = "D:\\QtProjects\\Opencv_Test2\\musicForProject\\surprise\\the_animals_-_boom_boom.mp3";
                    trackName = "The Animals - Boom Boom";
                    break;
                }
                break;
    }
    ui->musicTrackNameLabel->setText(trackName);
    QString str2 = QString::fromStdString(str1);
    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators(str2)));
    player->setVolume(50);
    qint64 playPosition = rand () % 15 + 20;
    playPosition *= 1000;
    player->play();
    player->setPosition(player->position() + playPosition);


}

void MainWindow::on_musicPlayBtn_clicked()
{
    player->play();
}

void MainWindow::on_musicPauseBtn_clicked()
{
    player->pause();
}

void MainWindow::on_musicMuteBtn_clicked()
{
    if (player->volume() == 50){
        player->setVolume(0);
    }
    else player->setVolume(50);
}
