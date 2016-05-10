#-------------------------------------------------
#
# Project created by QtCreator 2016-04-02T19:10:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Opencv_Test2
TEMPLATE = app

INCLUDEPATH += C:\Qt\OpenCV\include
LIBS +=-L"C:\Qt\OpenCV\x86\mingw\lib"
LIBS +=  \
-llibopencv_core310 \
-llibopencv_imgproc310 \
-llibopencv_highgui310 \
-llibopencv_ml310 \
-llibopencv_videoio310 \
-llibopencv_features2d310 \
-llibopencv_calib3d310 \
-llibopencv_objdetect310 \
-llibopencv_flann310 \
-llibopencv_imgcodecs310


SOURCES += main.cpp\
        mainwindow.cpp\
        cjson.c

HEADERS  += mainwindow.h\
        cjson.h

FORMS    += mainwindow.ui

