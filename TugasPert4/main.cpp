#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Qt Convex Hull Simulator");
    w.resize(800, 600);
    w.show();

    return a.exec();
}
