#include "mainwindow.h"
#include "chart.h"
#include "chartview.h"
#include <QtWidgets/QMainWindow>
#include <QtCore/QtMath>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QApplication>
#include "example.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.grabGesture(Qt::PanGesture);
    w.grabGesture(Qt::PinchGesture);
    w.show();
    return a.exec();
}




