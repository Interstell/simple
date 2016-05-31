#include "QTime"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    w.show();

    return a.exec();
}
