#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cjson.h"
#include <QBitmap>
#include <QRegion>
#include <Windows.h>


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
    VideoCapture cap;
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
        img.release();
    }

}


void MainWindow::on_stopBtn_clicked()
{
    MainWindow::toCapture = false;
}

double* MainWindow::emotions_Analyze(char* link){
    char cmd[600];
    char exportFileName[400];
    sprintf(exportFileName, "emotions.json");
    sprintf(cmd, "curl.exe -o %s -XPOST https://api.projectoxford.ai/emotion/v1.0/recognize?subscription-key=349f1c7ff1e642498d333cb2fe5fab25 -k --data-binary @%s -H \"Content-Type:application/octet-stream\"", exportFileName, link);
    //system(cmd);// WARNING! NO UPDATE IF COMMENTED
    WinExec(cmd,SW_HIDE);
    //system("cls");

    FILE* fJSON = fopen(exportFileName, "r");

    char jsonContent[1000];
    fread(jsonContent,sizeof(char),1000,fJSON);
    cJSON* root = cJSON_Parse(jsonContent);

    if (cJSON_GetArraySize(root) == 0){//no face detected

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

void MainWindow::on_emotionBtn_clicked()
{
    imwrite("img.jpg", MainWindow::img);
    double* emotions = MainWindow::emotions_Analyze("img.jpg");
    if (!emotions) return; //NULL
    char res[400];
    sprintf(res, "%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", emotions[0], emotions[1],emotions[2],emotions[3],emotions[4],emotions[5], emotions[6], emotions[7]);
    ui->emoLabel->setText(res);
    //ui->emoLabel->setText(res);
    /*QPixmap pixmap("mask1.png");
    //ui->emoLabel->setPixmap(pixmap);
    QBitmap mask = pixmap.createMaskFromColor(Qt::black, Qt::MaskOutColor);
    ui->emoLabel->setMask(mask);

    ui->emoLabel->setStyleSheet("QLabel {width: 200px; height: 100px;  background: red; -moz-border-radius: 100px / 50px; -webkit-border-radius: 100px / 50px; border-radius: 100px / 50px;}");*/

    //QRegion maskedRegion(ui->emoLabel->geometry().x(),ui->emoLabel->geometry().y(),ui->emoLabel->geometry().width(),ui->emoLabel->geometry().height(),QRegion::Ellipse);
    //ui->emoLabel->setMask(maskedRegion);

    int maxEmoIndex = 0;

    for (int i = 1; i<8; i++){
        if (emotions[i]>emotions[maxEmoIndex]){
            if (i == 5 && emotions[i] > 0.93) //correction for neutral
                maxEmoIndex = i;
        }
    }

    char bgColor[10];
    switch (maxEmoIndex){
    case 0: //anger
        strcpy(bgColor,"#FF0000"); //red
        break;
    case 1: //contempt
        strcpy(bgColor,"#800020"); //burgundy
        break;
    case 2: //disgust
        strcpy(bgColor,"#00FF00"); //green
        break;
    case 3: //fear
        strcpy(bgColor,"#9F00C5"); //purple
        break;
    case 4: //happiness
        strcpy(bgColor,"#FFFF00"); //yellow
        break;
    case 5: //neutral
        strcpy(bgColor,"#FF7F00"); //orange
        break;
    case 6: //sadness
        strcpy(bgColor,"#0000FF"); //blue
        break;
    case 7: //surprise
        strcpy(bgColor,"#FFC0CB"); //pink
        break;

    }
    char cssForLabel[100];
    sprintf(cssForLabel,"QLabel {border-radius: %dpx; background-color : %s;}", min(ui->emoLabel->geometry().width(),ui->emoLabel->geometry().height())/2, bgColor);
    ui->emoLabel->setStyleSheet(cssForLabel);

    delete[] emotions;
}
