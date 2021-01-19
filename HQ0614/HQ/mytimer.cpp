#include "mytimer.h"
#include <QtCore>
#include <QDebug>
#include "mainwindow.h"

myTimer::myTimer()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()) ,this, SLOT(MySlot()));
    timer->start(1000);
}
void myTimer::MySlot(){

    qDebug () << "Timer executed";

}
