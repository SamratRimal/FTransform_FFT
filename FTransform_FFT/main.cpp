#include "mainwindow.h"

#include <QApplication>
#include <qdebug.h>
#include <QPaintEvent>
#include <QPen>


#include <iostream>
#include <cmath>
#include <vector>
#include <complex>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
