#-------------------------------------------------
#
# Project created by QtCreator 2016-04-02T19:10:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Opencv_Test2
TEMPLATE = app


INCLUDEPATH += "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\OpenCV\\include"

CONFIG(debug,debug|release) {
LIBS += -L"C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\OpenCV\\x86\\vc12\\lib"
LIBS += -lopencv_core310d
LIBS += -lopencv_imgproc310d
LIBS += -lopencv_highgui310d
LIBS += -lopencv_ml310d
LIBS += -lopencv_videoio310d
LIBS += -lopencv_features2d310d
LIBS += -lopencv_calib3d310d
LIBS += -lopencv_objdetect310d
LIBS += -lopencv_flann310d
LIBS += -lopencv_imgcodecs310d
}

CONFIG(release,debug|release) {
LIBS += -L"C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\OpenCV\\x86\\vc12\\lib"
LIBS += -lopencv_core310
LIBS += -lopencv_imgproc310
LIBS += -lopencv_highgui310
LIBS += -lopencv_ml310
LIBS += -lopencv_videoio310
LIBS += -lopencv_features2d310
LIBS += -lopencv_calib3d310
LIBS += -lopencv_objdetect310
LIBS += -lopencv_flann310
LIBS += -lopencv_imgcodecs310
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
