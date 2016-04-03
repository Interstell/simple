#include "mainwindow.h"
#include "ui_mainwindow.h"



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

    while(1){
        Mat img;
        cap >> img;

        qimg = QPixmap::fromImage(QImage((unsigned char*) img.data,
                                          img.cols,
                                          img.rows,
                                          QImage::Format_RGB888).rgbSwapped());
        qimg = qimg.scaledToWidth(640);
        ui->label->setPixmap(qimg);
        ui->label->show();

        int input = waitKey(20); //getting user input
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
