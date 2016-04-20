#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <string.h>
#include <windows.h>





namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool toCapture = false;
    double* emotions_Analyze(char* link);
    cv::Mat img;
    ~MainWindow();

private slots:
    void on_startBtn_clicked();

    void on_stopBtn_clicked();



    void on_emotionBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
