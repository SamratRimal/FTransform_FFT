#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdebug.h>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>


#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
#include <QtGui>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    virtual void paintEvent (QPaintEvent *e);

};
#endif // MAINWINDOW_H
