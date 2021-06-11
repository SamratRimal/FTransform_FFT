#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QApplication>

#include <QPaintEvent>
#include <QPen>

#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
using namespace std;
//const float e = 2.7182818284590452353602874713527;
typedef std::complex<double> ComplexVector;

std::vector<ComplexVector>inputData;
std::vector<ComplexVector>xarray;
vector<ComplexVector>to_FFT;
int loopSize;
double angle =0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

          QFile file("C:/Users/user/Desktop/desk/DFTFun/Sristi3rd.json");

           file.open(QIODevice::ReadOnly | QIODevice::Text);
           QByteArray jsonData = file.readAll();
           file.close();

           QJsonDocument document = QJsonDocument::fromJson(jsonData);
           QJsonObject object = document.object();

           QJsonValue value = object.value("drawing");
           QJsonArray array = value.toArray();

    //getting each item of json
           for(int i = 0;i<2;i++){
               foreach(const QJsonValue & X, array){
                 double xVal =X.toObject().value("x").toVariant().toFloat();
                 double yVal =X.toObject().value("y").toVariant().toFloat();
                 inputData.push_back({(xVal),(yVal)});     //inputing the value of X to xArray with skipping
               }
           }


             loopSize = inputData.size();
             cout<<loopSize;


}

MainWindow::~MainWindow()
{
    delete ui;
}


vector<ComplexVector>FFT_calculation(vector<ComplexVector>Sample) {

    int N = Sample.size();
    int M = N / 2;

    if (N <= 1) {
        return Sample;
    }

    vector<ComplexVector>XYeven(M, 0);
    //	XYeven.reserve(M);
    vector<ComplexVector>XYodd(M, 0);
    //	XYodd.reserve(M);

    for (int cnt = 0; cnt < M; cnt++) {
        XYeven[cnt] = Sample[2 * cnt];
        XYodd[cnt] = Sample[2 * cnt + 1];
        //cout << cnt << " " << XYeven[cnt] << "  " << XYodd[cnt] << "\n";

    }
    vector<ComplexVector>Feven(M, 0);
    vector<ComplexVector>Fodd(M, 0);
    Feven = FFT_calculation(XYeven);
    Fodd = FFT_calculation(XYodd);

    double angle = -2 * M_PI;
    for (int k = 0; k < N / 2; k++) {
        ComplexVector t = polar(1.0, angle*k / N)*Fodd[k];
        Sample[k] = Feven[k] + t;
        Sample[k + N / 2] = Feven[k] - t;


    }
    return Sample;
}


vector<ComplexVector> iFFT(vector<ComplexVector> inverse)                    // Inverse Fast Fourier Transform
{


    int N = inverse.size();
    vector<ComplexVector> ftildeConjugate(N,0);
    vector<ComplexVector> afterFft(N,0);
    for (int m = 0; m < N; m++) ftildeConjugate[m] = conj(inverse[m]);

    afterFft =FFT_calculation(ftildeConjugate);

    double factor = 1.0 / N;
    for (int m = 0; m < N; m++) {
        afterFft[m] = conj(afterFft[m]) * factor;
    }
    return afterFft;
}

void MainWindow::paintEvent(QPaintEvent *e)
{

    setAutoFillBackground(0);  //for the rotations or movement of the screen

    QPainter painter(this);  //setting paitter to this window
    painter.translate(400,300);  //translating the First Circle in left downward
    float x_x = 0;
    float y_y = 0;
    double  Mx_x;
//    double  My_y;

     double re =0;
     double im =0;
    //inputing a complex array to animal Which gives the real and imaginary num of each element in balanceX
    std::vector<std::complex<double>> Average2 = iFFT(inputData);
    for (int cnt = 0; cnt < loopSize; cnt++) {
//        double prevX=x_x;
//        double prevY=y_y;

        double freq=0;
        float newradius=0;
        re = Average2[cnt].real();   //takes the real value from of each elements of Average
        im = Average2[cnt].imag();  // imaginery number for ech elements
        freq = cnt;             //frequency of each element
//        qDebug() << cnt << ".   im==" << im << "     re==" << re << "\n\n";
        double amp = sqrt((re*re) + (im * im));   // finding the radius of circle need to be form DFT formula
        double phase = atan2(im,re);  //finding the phase from DFT fourmula
//                cout <<cnt<< ".   radius==" << amp << "     phase==" << phase<<"\n";
        newradius = amp;  //taking ampitude as radius
        if(cnt==0){
            Mx_x = newradius* cos(freq*angle+phase+M_PI);   //sum of each elements cos and sin value
//           My_y = newradius * sin(freq*angle+phase+M_PI);
            qDebug()<<"Mx_x=="<<Mx_x;
        }
        x_x += newradius* cos(freq*angle+phase+2*M_PI);   //sum of each elements cos and sin value
        y_y += newradius * sin(freq*angle+phase+2*M_PI);

//        painter.drawEllipse(prevX-newradius,prevY-newradius,newradius*2,newradius*2);
//        painter.drawLine(prevX,prevY,x_x,y_y);

//        // cout<<"circleValue"<<amp<<"\n";
     }
     xarray.push_back({x_x,y_y});
     qDebug()<<x_x<<","<<x_x;

    painter.translate(100,300);
    //drawing lines fromlast circle to the drawing part
    painter.drawLine(x_x-100,y_y-300,xarray[(int(xarray.size()-1))].real()-100,xarray[(int(xarray.size()-1))].imag()-300);

    QPen pen;  //Dradient to make points or line color Blue
    QRadialGradient gradient;
    gradient.setColorAt(1, Qt::Key_Blue);
//    gradient.setColorAt(1, QColor::fromRgbF(11, 0, 0));

  //  QBrush brush(gradient);
    pen.setWidth(2);  //setting the width of dots
    pen.setBrush(gradient);
    painter.setPen(pen);
    painter.translate(-100,-300);
    //printing the last value first (unsifting value)
    for (int cnt =int(xarray.size()-1);cnt>=1;cnt--){
//        painter.drawPoint(double(xarray[cnt].real()),double(xarray[cnt].imag()));
        painter.drawLine(double(xarray[cnt-1].real()),double(xarray[cnt-1].imag()),double(xarray[cnt].real()),double(xarray[cnt].imag()));
//        cout<<"Xvalue"<<(xarray[cnt])<<"\n";
    }

    setAutoFillBackground(255);

    const float dt = 2*M_PI/loopSize;   //time for rotation must be two_Pi/ number of item in input array
    angle +=dt;
    qDebug()<<"angle =="<<angle;

    if(angle>2*M_PI){  //if full cycle is made the  clears array and set angle zero
        angle =0;
        xarray.clear();
        xarray.clear();

    }


}


