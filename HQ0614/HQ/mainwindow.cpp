#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chart.h"
#include "chartview.h"
#include <QtWidgets/QMainWindow>
#include <QtCore/QtMath>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts>
#include <QtGui>
#include <QtCore>
using namespace QtCharts;
using namespace rapidjson;
#include "http.h"
#include <example.h>
#include <QtMath>
#include <QNetworkReply>
#include <QtWidgets>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include <iostream>
#include <stdio.h>
char legend_list[] = "abca";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_netManager = new QNetworkAccessManager(this);
    m_netManager_2 = new QNetworkAccessManager(this);
    now_loc = 4096;

    label2 =new QLabel();
    label2->setText("RMS of Vab :");
    label3 =new QLabel();
    label3->setText("RMS of Vbc :");
    label4 =new QLabel();
    label4->setText("RMS of Vca :");
    label5 =new QLabel();
    label5->setText("RMS of Ia :");
    label6 =new QLabel();
    label6->setText("RMS of Ib :");
    label7 =new QLabel();
    label7->setText("RMS of Ic :");



    //
    //char legend_list[] = "abc";
    //

    for(int i =0;i<6;i++){
        series[i] = new QSplineSeries();
        series[i]->setUseOpenGL(true);
        m_series.append(series[i]);
        if(i<3){
            series[i]->setName(QString("Voltage Line " + QString(legend_list[i])+ QString(legend_list[i+1])));
        }else{
            series[i]->setName(QString("Current Line " + QString(legend_list[i-3])+QString(legend_list[i-2])));
        }

    }

    length = 4096;

    for(int i=0;i<6;i++){
        p[i] = &data[i][0];
        fft_p[i] = &data_fft[i][0];
    }
    //p[0] = {&data[0][0],&data[1][0],&data[2][0],&data[3][0],&data[4][0],&data[5][0]};
    //fft_p[] = {&data_fft[0][0],&data_fft[1][0],&data_fft[2][0],&data_fft[3][0],&data_fft[4][0],&data_fft[5][0]};

    for(int j=0;j<6;j++){
        readcsv(p[j],j);
        for (unsigned int i = 0; i < length; i++) {
            series[j]->append(i, data[j][i]);

        }
    }

    for(int j=0;j<6;j++){
        for(unsigned int i=0;i<length;i++){
            data_fft[j][i].real = data[j][i];
            data_fft[j][i].imag = 0;
        }
    }
    //rms


    for(int i=0;i<6;i++){
        if(i<3){
            Vrms[i] = rms(data[i],length);
        }else{
            Irms[i-3] = rms(data[i],length);
        }
    }

    rms_avg[0] += (Vrms[0]+Vrms[1]+Vrms[2])/3;
    rms_avg[1] += (Irms[0]+Irms[1]+Irms[2])/3;

    L_V_U_R = max_distance(Vrms,3,rms_avg[0])/rms_avg[0];
    L_I_U_R = max_distance(Irms,3,rms_avg[1])/rms_avg[1];
    //

    label2->setText(label2->text().append(QString::number(Vrms[0]).append(" Volt")));
    label3->setText(label3->text().append(QString::number(Vrms[1]).append(" Volt")));
    label4->setText(label4->text().append(QString::number(Vrms[2]).append(" Volt")));
          chart = new Chart();
          chart->legend()->setVisible(true);
          chart->addSeries(series[0]);
          chart->addSeries(series[1]);
          chart->addSeries(series[2]);
          chart->createDefaultAxes();
          chart->setTitle("Voltage");
          connectMarkers(chart);

          chartView = new ChartView(chart);
          chartView->setRenderHint(QPainter::Antialiasing);

    this->ui->verticalLayout->addWidget(chartView);



          label5->setText(label5->text().append(QString::number(Irms[0]).append(" Amp")));
          label6->setText(label6->text().append(QString::number(Irms[1]).append(" Amp")));
          label7->setText(label7->text().append(QString::number(Irms[2]).append(" Amp")));
          chart2 = new Chart();
          chart2->legend()->setVisible(true);
          chart2->addSeries(series[3]);
          chart2->addSeries(series[4]);
          chart2->addSeries(series[5]);
          chart2->createDefaultAxes();
          chart2->setTitle("Current");
          connectMarkers(chart2);

          QChartView *chartView2 = new ChartView(chart2);
          chartView2->setRenderHint(QPainter::Antialiasing);

    this->ui->verticalLayout->addWidget(chartView2);



    this->ui->verticalLayout_2->addWidget(label2);
    this->ui->verticalLayout_2->addWidget(label3);
    this->ui->verticalLayout_2->addWidget(label4);
    this->ui->verticalLayout_2->addWidget(label5);
    this->ui->verticalLayout_2->addWidget(label6);
    this->ui->verticalLayout_2->addWidget(label7);


    //LUR
    label9 = new QLabel();
    label9->setText("線電壓不平衡率: ");
    label9->setText(label9->text().append(QString::number(L_V_U_R*100).append(" %")));
    this->ui->verticalLayout_2->addWidget(label9);
    label10 = new QLabel();
    label10->setText("線電流不平衡率: ");
    label10->setText(label10->text().append(QString::number(L_I_U_R*100).append(" %")));
    this->ui->verticalLayout_2->addWidget(label10);
    //Variation
    label11 = new QLabel();
    label11->setText("Vab電壓偏差率 :");
    label12 = new QLabel();
    label12->setText("Vbc電壓偏差率 :");
    label13 = new QLabel();
    label13 ->setText("Vca電壓偏差率 :");

    label14 = new QLabel();
    label14->setText("Ia電流偏差率 :");
    label15 = new QLabel();
    label15->setText("Ib電流偏差率 :");
    label16 = new QLabel();
    label16->setText("Ic電流偏差率 :");

    label11->setText(label11->text().append(QString::number(qFabs(Vrms[0]-220)*100/220)).append(" %"));
    this->ui->verticalLayout_3->addWidget(label11);
    label12->setText(label12->text().append(QString::number(qFabs(Vrms[1]-220)*100/220)).append(" %"));
    this->ui->verticalLayout_3->addWidget(label12);
    label13->setText(label13->text().append(QString::number(qFabs(Vrms[2]-220)*100/220)).append(" %"));
    this->ui->verticalLayout_3->addWidget(label13);
    label14->setText(label14->text().append(QString::number(qFabs(Irms[0]-0.6)*100/0.6)).append(" %"));
    this->ui->verticalLayout_3->addWidget(label14);
    label15->setText(label15->text().append(QString::number(qFabs(Irms[1]-0.6)*100/0.6)).append(" %"));
    this->ui->verticalLayout_3->addWidget(label15);
    label16->setText(label16->text().append(QString::number(qFabs(Irms[2]-0.6)*100/0.6)).append(" %"));
    this->ui->verticalLayout_3->addWidget(label16);
    //FFT

    for(int i=0;i<6;i++){
        FFT_length(fft_p[i],length);
    }


    for(int j=0;j<6;j++){
    for(int i=0;i<length;i++){
            abs_data_fft[j][i] = sqrt(data_fft[j][i].real*data_fft[j][i].real+data_fft[j][i].imag*data_fft[j][i].imag)/length;
        }
    }
    int s_index[6];

    for(int i=0;i<6;i++){
        s_index[i] = max_index(abs_data_fft[i],length,0);
        //qDebug ()<<s_index[i];
    }

    for(int i =0;i<6;i++){
        series_fft[i] = new QSplineSeries();
        series_fft[i]->setUseOpenGL(true);
    }

    for(int j =0;j<6;j++){
        for(int i =0;i<length/2;i++){
            series_fft[j]->append(i,abs_data_fft[j][i]);
            m_series.append(series_fft[j]);
            if(j<3){
                series_fft[j]->setName(QString("Voltage Line " + QString(legend_list[i])));
            }else{
                series_fft[j]->setName(QString("Current Line " + QString(legend_list[i-3])));
            }
        }
    }
    chart3 = new QChart();

    chart3->addSeries(series_fft[0]);
    chart3->addSeries(series_fft[1]);
    chart3->addSeries(series_fft[2]);
    connectMarkers(chart3);
    chart3->legend()->setVisible(true);
    chart3->createDefaultAxes();

    chart4 = new QChart();
    chart4->addSeries(series_fft[3]);
    chart4->addSeries(series_fft[4]);
    chart4->addSeries(series_fft[5]);
    connectMarkers(chart4);
    chart4->legend()->setVisible(true);
    chart4->createDefaultAxes();

    QChartView *chartView3 = new ChartView(chart3);
    chartView3->setRenderHint(QPainter::Antialiasing);
    QChartView *chartView4 = new ChartView(chart4);
    chartView4->setRenderHint(QPainter::Antialiasing);


    this->ui->verticalLayout_5->addWidget(chartView3);
    this->ui->verticalLayout_5->addWidget(chartView4);
//polar
    const qreal angularMin = -1;
    const qreal angularMax = 359;

    const qreal radialMin = 0;
    const qreal radialMax = 250;

    const qreal C_radialMin =0;
    const qreal C_radialMax =200;


    freq_index  = max_index(abs_data_fft[0],length,0);
    if(freq_index !=60){
        qDebug () << "shit went down!\n";
    }

    for(int i=0;i<6;i++){
      polar_angular[i] =  arctan_2pi(data_fft[i][freq_index]);
      if(i==0){
          //vab=0
          polar_cail_angular[i] = 0;

      }else{
          //vx = vx - vab0
          polar_cail_angular[i] = polar_angular[i] - polar_angular[0];
          if(polar_cail_angular[i]<0.0){
              polar_cail_angular[i] = polar_cail_angular[i]+360;
          }
      }
    }

    for(int i=0;i<6;i++){
        polar_radian[i] =  qDegreesToRadians(polar_cail_angular[i]);
    }
    //voltage
    polar_series1 = new QSplineSeries();
    polar_series1->setName("Vab");
    polar_series1->append(0,0);

    polar_series1->append(polar_cail_angular[0],Vrms[0]);
    m_series.append(polar_series1);

    polar_series2 = new QSplineSeries();
    polar_series2->setName("Vbc");
    polar_series2->append(0,0);
    polar_series2->append(polar_cail_angular[1],Vrms[1]);
    m_series.append(polar_series2);
    polar_series3 = new QSplineSeries();
    polar_series3->setName("Vca");
    polar_series3->append(0,0);
    polar_series3->append(polar_cail_angular[2],Vrms[2]);
    m_series.append(polar_series3);
    //current
    polar_series4 = new QSplineSeries();
    polar_series4->setName("Ia");
    polar_series4->append(0,0);
    polar_series4->append(polar_cail_angular[3],Irms[0]*400);
    m_series.append(polar_series4);

    polar_series5 = new QSplineSeries();
    polar_series5->setName("Ib");
    polar_series5->append(0,0);
    polar_series5->append(polar_cail_angular[4],Irms[1]*400);
    m_series.append(polar_series5);

    polar_series6 = new QSplineSeries();
    polar_series6->setName("Ic");
    polar_series6->append(0,0);
    polar_series6->append(polar_cail_angular[5],Irms[2]*400);
    m_series.append(polar_series6);


    //![1]
    polar_chart = new QPolarChart();
    //![1]
    polar_chart->addSeries(polar_series1);
    polar_chart->addSeries(polar_series2);
    polar_chart->addSeries(polar_series3);
    polar_chart->addSeries(polar_series4);
    polar_chart->addSeries(polar_series5);
    polar_chart->addSeries(polar_series6);


    connectMarkers(polar_chart);
    polar_chart->legend()->setVisible(true);
    polar_chart->setTitle("Use arrow keys to scroll, +/- to zoom, and space to switch chart type.Voltage x1. Current x400.");

    //![2]
    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setTickCount(9); // First and last ticks are co-located on 0/360 angle.
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    polar_chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    QValueAxis *radialAxis = new QValueAxis();
    QValueAxis *C_radialAxis = new QValueAxis();
    C_radialAxis->setLabelsAngle(90);

    C_radialAxis->setTickCount(9);

    radialAxis->setTickCount(9);
    radialAxis->setLabelFormat("%d");
    polar_chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    //polar_chart->addAxis(C_radialAxis,QPolarChart::PolarOrientationRadial);
    //![2]

    polar_series1->attachAxis(radialAxis);
    polar_series1->attachAxis(angularAxis);
    polar_series2->attachAxis(radialAxis);
    polar_series2->attachAxis(angularAxis);
    polar_series3->attachAxis(radialAxis);
    polar_series3->attachAxis(angularAxis);
    polar_series4->attachAxis(radialAxis);
    polar_series4->attachAxis(angularAxis);
    polar_series5->attachAxis(radialAxis);
    polar_series5->attachAxis(angularAxis);
    polar_series6->attachAxis(radialAxis);
    polar_series6->attachAxis(angularAxis);


    radialAxis->setRange(radialMin, radialMax);
    C_radialAxis->setRange(C_radialMin,C_radialMax);
    angularAxis->setRange(angularMin, angularMax);



    QChartView *polar_chartView = new ChartView(polar_chart);
    //polar_chartView->setChart(polar_chart);
    polar_chartView->setRenderHint(QPainter::Antialiasing);
    this->ui->gridLayout_5->addWidget(polar_chartView);
//vuf


    for(int i=0;i<3;i++){
        v_compx[i].real = Vrms[i]*qCos(polar_radian[i]);
        i_compx[i].real = Irms[i]*qCos(polar_radian[i+3]);
        v_compx[i].imag = Vrms[i]*qSin(polar_radian[i]);
        i_compx[i].imag = Irms[i]*qSin(polar_radian[i+3]);

       qDebug () << v_compx[i].real<<"+j" <<v_compx[i].imag<<"with angular"<<polar_radian[i];
       qDebug () << i_compx[i].real<<"+j" <<i_compx[i].imag<<"with angular"<<polar_radian[i+3];

    }

    compx t1,v1,v2,i1,i2={0,0};
    compx a ={-0.5,0.8660254038};
    compx asquare = EE(a,a);
    //v1
    t1 = v_compx[0];
    v1.real += t1.real/3;
    v1.imag += t1.imag/3;
    t1 = EE(a,v_compx[1]);
    v1.real += t1.real/3;
    v1.imag += t1.imag/3;
    t1 = EE(asquare,v_compx[2]);
    v1.real += t1.real/3;
    v1.imag += t1.imag/3;
    //v2
    t1 = v_compx[0];
    v2.real += t1.real/3;
    v2.imag += t1.imag/3;
    t1 = EE(asquare,v_compx[1]);
    v2.real += t1.real/3;
    v2.imag += t1.imag/3;
    t1 = EE(a,v_compx[2]);
    v2.real += t1.real/3;
    v2.imag += t1.imag/3;

//    qDebug () << v1.real;
//    qDebug () << v1.imag;
//    qDebug () << v2.real;
//    qDebug () << v2.imag;
    compx v1_comp = {v1.real,-1*v1.imag};

    compx v2_comp = {v2.real,-1*v2.imag};
    compx v1_abs = EE(v1,v1_comp);
    compx v2_abs = EE(v2,v2_comp);
    v1_abs.real = qSqrt(v1_abs.real);
    v2_abs.real = qSqrt(v2_abs.real);
//    qDebug () << v1_abs.real <<"+j"<<v1_abs.imag;
//    qDebug () << v2_abs.real <<"+j"<<v2_abs.imag;
    VUF = (v1_abs.real/v2_abs.real)*100;
    label17 = new QLabel();
    label17->setText("電壓不平衡因素 :");
    label17->setText(label17->text().append(QString::number(VUF)).append(" %"));
    this->ui->verticalLayout_3->addWidget(label17);

    //current
    //i1
    t1 = i_compx[0];
    i1.real += t1.real/3;
    i1.imag += t1.imag/3;
    t1 = EE(a,i_compx[1]);
    i1.real += t1.real/3;
    i1.imag += t1.imag/3;
    t1 = EE(asquare,i_compx[2]);
    i1.real += t1.real/3;
    i1.imag += t1.imag/3;
    //v2
    t1 = i_compx[0];
    i2.real += t1.real/3;
    i2.imag += t1.imag/3;
    t1 = EE(asquare,i_compx[1]);
    i2.real += t1.real/3;
    i2.imag += t1.imag/3;
    t1 = EE(a,i_compx[2]);
    i2.real += t1.real/3;
    i2.imag += t1.imag/3;
    qDebug () << "excel";
    qDebug () << i1.real;
    qDebug () << i1.imag;
    qDebug () << i2.real;
    qDebug () << i2.imag;

    compx i1_comp = {i1.real,-1*i1.imag};

    compx i2_comp = {i2.real,-1*i2.imag};
    compx i1_abs = EE(i1,i1_comp);
    compx i2_abs = EE(i2,i2_comp);
    i1_abs.real = qSqrt(i1_abs.real);
    i2_abs.real = qSqrt(i2_abs.real);

    IUF = (i1_abs.real/i2_abs.real)*100;

    label18 = new QLabel();
    label18->setText("電流不平衡因素 :");
    label18->setText(label18->text().append(QString::number(IUF)).append(" %"));
    this->ui->verticalLayout_3->addWidget(label18);

    //THD

    counter = (length / freq_index)+1;

    for(int j =0;j<6;j++){
        double thd_sum=0;
        for(int i=2;i< counter ;i++){
            thd_sum += abs_data_fft[j][i*60]*abs_data_fft[j][i*60];
        }
        thd_ans[j] = (qSqrt(thd_sum)/abs_data_fft[j][60])*100;
    }


    char thd[] ="abcaabca";

    for(int i=0;i<6;i++){
        label_thd[i]= new QLabel();
        if(i<3){
            label_thd[i]->setText(QString(thd[i])+QString(thd[i+1]).append(QString("線總電壓諧波失真率 :").append(QString::number(thd_ans[i]).append(" %"))));
        }else{
            label_thd[i]->setText(QString(thd[i+1]).append(QString("線總電流諧波失真率 :").append(QString::number(thd_ans[i]).append(" %"))));
        }
        this->ui->verticalLayout_3->addWidget(label_thd[i]);
    }
    //Power

    compx minus = {-1.0,0};


    compx vac = EE(v_compx[2],minus);
    compx Ia_conj = {i_compx[0].real,-1*(i_compx[0].imag)};
    compx Ib_conj = {i_compx[1].real,-1*(i_compx[1].imag)};
//    qDebug () << "vac" <<vac.real << "+j" << vac.imag;
//    qDebug () << "vbc" <<v_compx[1].real << "+j" << v_compx[1].imag;
//    qDebug () << "ia*" <<Ia_conj.real << "+j" << Ia_conj.imag;
//    qDebug () << "ib*" <<Ib_conj.real << "+j" << Ib_conj.imag;
    compx S = {0,0};
    compx t3 = EE(vac,Ia_conj);
    S.real = t3.real;
    S.imag = t3.imag;
    t3 = EE(v_compx[1],Ib_conj);
    S.real += t3.real;
    S.imag += t3.imag;

//    qDebug () << S.real << "+j" << S.imag;
//    double r1_3[3];
//    r1_3[0] = polar_radian[0]-polar_radian[3];
//    r1_3[1] = polar_radian[1]-polar_radian[4];
//    r1_3[2] = polar_radian[2]-polar_radian[5];



//    double P=0,Q=0;
//    for(int i=0;i<3;i++){
//        P += Irms[i]*v_amp[i]*qCos(r1_3[i]);
//        Q += Irms[i]*v_amp[i]*qSin(r1_3[i]);
//        qDebug () << r1_3[i];
//    }
//    P = P/qSqrt(3);
//    Q = Q/qSqrt(3);
    double P = S.real;
    double Q = S.imag;
    double IAP = qSqrt(S.real*S.real+S.imag*S.imag);
    double PF = P/IAP;
    double pwr_ans[4] = {P,Q,IAP,PF};
    QString pwr[4] = {"實功 :","虛功 :","視在功率 :","三項功率因數 :"};


    for(int i=0;i<4;i++){
        label_pwr[i]= new QLabel();
        label_pwr[i]->setText(QString(pwr[i]).append(QString::number(pwr_ans[i])));
        this->ui->verticalLayout_2->addWidget(label_pwr[i]);
    }


    QLabel *label8 = new QLabel();
    this->ui->verticalLayout_2->addWidget(label8);


    //

    now_index = new QLabel();
    this->ui->verticalLayout_4->addWidget(now_index);
    //
//    QPointF point = series[0]->at(100);
//    QPointF pointPos =  chart->mapToPosition(point);



    //http();
    //error detection
    if(Vrms[0]>220){
        label2->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Vrms[0]<200){
        label2->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }
    if(Vrms[1]>220){
        label3->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Vrms[1]<200){
        label3->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }
    if(Vrms[2]>220){
        label4->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Vrms[2]<200){
        label4->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }
    if(Irms[0]>0.6){
        label5->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Irms[0]<0.48){
        label5->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }
    if(Irms[1]>0.6){
        label6->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Irms[1]<0.48){
        label6->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }
    if(Irms[2]>0.6){
        label7->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Irms[2]<0.48){
        label7->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }
    if(L_V_U_R>0.1){
        label9->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }
    if(L_I_U_R>0.1){
        label10->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }
    if((qFabs(Vrms[0]-220)*100/220)>1){
        label11->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }
    if((qFabs(Vrms[1]-220)*100/220)>1){
        label12->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }
    if((qFabs(Vrms[2]-220)*100/220)>1){
        label13->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }
    if((qFabs(Irms[0]-0.6)*100/0.6)>25){
        label14->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }
    if((qFabs(Irms[1]-0.6)*100/0.6)>25){
        label15->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }
    if((qFabs(Irms[2]-0.6)*100/0.6)>25){
        label16->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }
    if(VUF>10){
        label17->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }
    if(IUF>10){
        label18->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }

    for(int i=0;i<6;i++){
        if(thd_ans[i]>1.5){
            label_thd[i]->setStyleSheet("QLabel { background-color : red; color : blue; }");
        }
    }
    //
}

void MainWindow::connectMarkers(QChart* chart)
{
//![1]
    // Connect all markers to handler
    const auto markers = chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWindow::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &MainWindow::handleMarkerClicked);
    }
//![1]
}
void MainWindow::disconnectMarkers()
{
//![2]
    const auto markers = chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWindow::handleMarkerClicked);
    }
//![2]
}
void MainWindow::handleMarkerClicked()
{
//![3]
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
//![3]

//![4]
    switch (marker->type())
//![4]
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
//![5]
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);
//![5]

//![6]
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible())
            alpha = 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

//![6]
        break;
        }
    default:
        {
        qDebug () << "Unknown marker type";
        break;
        }
    }
}

void MainWindow::removeSeries(QList<QSplineSeries *> m_series, QChart* chart)
{
    // Remove last series from chart
    if (m_series.count() > 0) {
        QSplineSeries *series_t = m_series.last();
        chart->removeSeries(series_t);
        m_series.removeLast();
        delete series_t;
    }
}





//enum
//{
//    REQ_GET,
//    REQ_POST
//};
bool first_set=false;
void MainWindow::doRequest(int reqType)
{
    QNetworkRequest request;
    QNetworkRequest request2;

    request.setUrl(QUrl(ui->lineEdit_1->text()));
    request2.setUrl(QString("http://admin:couchdb@26.189.86.65:5984/qt555/"));

    m_replyGotError = false;

    switch(reqType)
    {
    case REQ_GET:
        m_reply = m_netManager->get(request);
        break;
    case REQ_POST:{

        json.insert("RMS of Vab",QString::number(Vrms[0]));
        json.insert("RMS of Vbc",QString::number(Vrms[1]));
        json.insert("RMS of Vca",QString::number(Vrms[2]));
        json.insert("RMS of Ia",QString::number(Irms[0]));
        json.insert("RMS of Ib",QString::number(Irms[1]));
        json.insert("RMS of Ic",QString::number(Irms[2]));
        json.insert("IUF",QString::number(IUF));
        json.insert("VUF",QString::number(VUF));
        QJsonDocument jsonDoc(json);
        QByteArray jsonData= jsonDoc.toJson();
        request2.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
        request2.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(jsonData.size()));

        m_reply2 = m_netManager_2->post(request2, jsonData);
        qDebug() << "i did this part";
        break;
        }
    default:
        return;
    }
    if(reqType==REQ_POST){
    connect(m_reply2, SIGNAL(finished()), this, SLOT(ReplyFinished2()));
    connect(m_reply2, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(ReplyError2(QNetworkReply::NetworkError)));
    connect(m_reply2, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(ReplySSLError2(QList<QSslError>)));
    }else{
    connect(m_reply, SIGNAL(finished()), this, SLOT(ReplyFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(ReplyError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(ReplySSLError(QList<QSslError>)));
    }


}

void MainWindow::ReplyFinished()
{
    ui->headerText->setPlainText("");
    QList<QByteArray> headerList = m_reply->rawHeaderList();
    for(int i = 0; i < headerList.length(); ++i)
    {
        QString headerNameValuePair(headerList[i]);
        headerNameValuePair += " :       ";
        headerNameValuePair += m_reply->rawHeader(headerList[i]);
        headerNameValuePair += "\n";

        ui->headerText->appendPlainText(headerNameValuePair);
    }
    QString source =  QString(m_reply->readAll());


    document.Parse(source.toStdString().c_str());

    const rapidjson::Value& a = document["array"];

    #pragma omp parallel for num_threads(6)
    for(int j =0;j<6;j++){
        for(int i =0;i<4096;i++){
          data[0][i] = 0;
        }
//        series[j]->clear();
    }

    #pragma omp parallel for num_threads(6)
    for(int j =0;j<6;j++){
        for(int i =0;i<4096;i++){
            if(j<3){
                data[j][i]=a[j+i*6].GetFloat()*80;
                data_fft[j][i].real = data[j][i];
                data_fft[j][i].imag = 0;
            }else{
                data[j][i]=a[j+i*6].GetFloat();
                data_fft[j][i].real = data[j][i];
                data_fft[j][i].imag = 0;
            }

        }
        qDebug () << "this is data[" << j << "][0] :" <<data[j][0];
    }



    for (unsigned int i = 0; i <256; i++) {
        now_loc = now_loc+1;
        series[0]->append(now_loc,data[0][i]);
        series[1]->append(now_loc,data[1][i]);
        series[2]->append(now_loc,data[2][i]);
        series[3]->append(now_loc,data[3][i]);
        series[4]->append(now_loc,data[4][i]);
        series[5]->append(now_loc,data[5][i]);
       now_index->setText(QString::number(now_loc));
    }
    //
    #pragma omp parallel for num_threads(6)
    for(int i=0;i<6;i++){
        if(i<3){
            Vrms[i] = rms(data[i],length);
        }else{
            Irms[i-3] = rms(data[i],length);
        }
    }
    QString RMS_A = "RMS of Vab :";
    QString RMS_B = "RMS of Vbc :";
    QString RMS_C = "RMS of Vca :";
    QString RMS_IA= "RMS of Ia :";
    QString RMS_IB= "RMS of Ib :";
    QString RMS_IC= "RMS of Ic :";

    label2->setText(RMS_A.append(QString::number(Vrms[0]).append(" Volt")));
    label3->setText(RMS_B.append(QString::number(Vrms[1]).append(" Volt")));
    label4->setText(RMS_C.append(QString::number(Vrms[2]).append(" Volt")));
    label5->setText(RMS_IA.append(QString::number(Irms[0]).append(" Amp")));
    label6->setText(RMS_IB.append(QString::number(Irms[1]).append(" Amp")));
    label7->setText(RMS_IC.append(QString::number(Irms[2]).append(" Amp")));
    rms_avg[0] = 0;
    rms_avg[1] = 0;

    rms_avg[0] += (Vrms[0]+Vrms[1]+Vrms[2])/3;
    rms_avg[1] += (Irms[0]+Irms[1]+Irms[2])/3;
    L_V_U_R = max_distance(Vrms,3,rms_avg[0])/rms_avg[0];
    L_I_U_R = max_distance(Irms,3,rms_avg[1])/rms_avg[1];
    for(int i=0;i<6;i++){
        FFT_length(fft_p[i],length);
    }
    for(int j=0;j<6;j++){
    for(int i=0;i<length;i++){
            abs_data_fft[j][i] = sqrt(data_fft[j][i].real*data_fft[j][i].real+data_fft[j][i].imag*data_fft[j][i].imag)/length;
        }
    }
    for(int j =0;j<6;j++){
        series_fft[j]->clear();
        for(int i =0;i<length/2;i++){
            series_fft[j]->append(i,abs_data_fft[j][i]);
        }
    }
    freq_index  = max_index(abs_data_fft[0],length,0);
    if(freq_index !=60){
        qDebug () << "shit went down!\n";
    }

    for(int i=0;i<6;i++){
      polar_angular[i] =  arctan_2pi(data_fft[i][freq_index]);
      if(i==0){
          //vab=0
          polar_cail_angular[i] = 0;

      }else{
          //vx = vx - vab0
          polar_cail_angular[i] = polar_angular[i] - polar_angular[0];
          if(polar_cail_angular[i]<0.0){
              polar_cail_angular[i] = polar_cail_angular[i]+360;
          }
      }
    }

    for(int i=0;i<6;i++){
        polar_radian[i] =  qDegreesToRadians(polar_cail_angular[i]);
        qDebug () << "radian" << polar_radian[i];
    }

    polar_series1->clear();
    polar_series2->clear();
    polar_series3->clear();
    polar_series4->clear();
    polar_series5->clear();
    polar_series6->clear();
    polar_series1->append(0,0);
    polar_series2->append(0,0);
    polar_series3->append(0,0);
    polar_series4->append(0,0);
    polar_series5->append(0,0);
    polar_series6->append(0,0);

    polar_series1->append(polar_cail_angular[0],Vrms[0]);
    polar_series2->append(polar_cail_angular[1],Vrms[1]);
    polar_series3->append(polar_cail_angular[2],Vrms[2]);
    polar_series4->append(polar_cail_angular[3],Irms[0]*400);
    polar_series5->append(polar_cail_angular[4],Irms[1]*400);
    polar_series6->append(polar_cail_angular[5],Irms[2]*400);


    for(int i=0;i<3;i++){
        v_compx[i].real = Vrms[i]*qCos(polar_radian[i]);
        i_compx[i].real = Irms[i]*qCos(polar_radian[i+3]);
        v_compx[i].imag = Vrms[i]*qSin(polar_radian[i]);
        i_compx[i].imag = Irms[i]*qSin(polar_radian[i+3]);

       //qDebug () << v_compx[i].real<<"+j" <<v_compx[i].imag<<"with angular"<<polar_radian[i];
       //qDebug () << i_compx[i].real<<"+j" <<i_compx[i].imag<<"with angular"<<polar_radian[i+3];

    }

    compx t1,v1,v2,i1,i2={0,0};
    compx alpha ={-0.5,0.8660254038};
    compx asquare = EE(alpha,alpha);
    //v1
    t1 = v_compx[0];
    v1.real += t1.real/3;
    v1.imag += t1.imag/3;
    t1 = EE(alpha,v_compx[1]);
    v1.real += t1.real/3;
    v1.imag += t1.imag/3;
    t1 = EE(asquare,v_compx[2]);
    v1.real += t1.real/3;
    v1.imag += t1.imag/3;
    //v2
    t1 = v_compx[0];
    v2.real += t1.real/3;
    v2.imag += t1.imag/3;
    t1 = EE(asquare,v_compx[1]);
    v2.real += t1.real/3;
    v2.imag += t1.imag/3;
    t1 = EE(alpha,v_compx[2]);
    v2.real += t1.real/3;
    v2.imag += t1.imag/3;

//    qDebug () << v1.real;
//    qDebug () << v1.imag;
//    qDebug () << v2.real;
//    qDebug () << v2.imag;
    compx v1_comp = {v1.real,-1*v1.imag};

    compx v2_comp = {v2.real,-1*v2.imag};
    compx v1_abs = EE(v1,v1_comp);
    compx v2_abs = EE(v2,v2_comp);
    v1_abs.real = qSqrt(v1_abs.real);
    v2_abs.real = qSqrt(v2_abs.real);
//    qDebug () << v1_abs.real <<"+j"<<v1_abs.imag;
//    qDebug () << v2_abs.real <<"+j"<<v2_abs.imag;
    VUF = (v1_abs.real/v2_abs.real)*100;
    label17->setText("電壓不平衡因素 :");
    label17->setText(label17->text().append(QString::number(VUF)).append(" %"));

    //current
    //i1
    t1 = i_compx[0];
    i1.real += t1.real/3;
    i1.imag += t1.imag/3;
    t1 = EE(alpha,i_compx[1]);
    i1.real += t1.real/3;
    i1.imag += t1.imag/3;
    t1 = EE(asquare,i_compx[2]);
    i1.real += t1.real/3;
    i1.imag += t1.imag/3;
    //v2
    t1 = i_compx[0];
    i2.real += t1.real/3;
    i2.imag += t1.imag/3;
    t1 = EE(asquare,i_compx[1]);
    i2.real += t1.real/3;
    i2.imag += t1.imag/3;
    t1 = EE(alpha,i_compx[2]);
    i2.real += t1.real/3;
    i2.imag += t1.imag/3;
    qDebug () << "pratical";
    qDebug () << i1.real;
    qDebug () << i1.imag;
    qDebug () << i2.real;
    qDebug () << i2.imag;

    compx i1_comp = {i1.real,-1*i1.imag};

    compx i2_comp = {i2.real,-1*i2.imag};
    compx i1_abs = EE(i1,i1_comp);
    compx i2_abs = EE(i2,i2_comp);
    i1_abs.real = qSqrt(i1_abs.real);
    i2_abs.real = qSqrt(i2_abs.real);

    IUF = (i1_abs.real/i2_abs.real)*100;

    label18->setText("電流不平衡因素 :");
    label18->setText(label18->text().append(QString::number(IUF)).append(" %"));
    //THD

    if(freq_index<10){
        freq_index=10;
        ui->statusbar->showMessage(QString("Detect none signal from server!"),10000);
    }
    counter = (length / freq_index)+1;

    for(int j =0;j<6;j++){
        double thd_sum=0;
        for(int i=2;i< counter ;i++){
            thd_sum += abs_data_fft[j][i*60]*abs_data_fft[j][i*60];
        }
        thd_ans[j] = (qSqrt(thd_sum)/abs_data_fft[j][60])*100;
    }


    char thd[] ="abcaabca";

    for(int i=0;i<6;i++){
        if(i<3){
            label_thd[i]->setText(QString(thd[i])+QString(thd[i+1]).append(QString("線總電壓諧波失真率 :").append(QString::number(thd_ans[i]).append(" %"))));
        }else{
            label_thd[i]->setText(QString(thd[i+1]).append(QString("線總電流諧波失真率 :").append(QString::number(thd_ans[i]).append(" %"))));
        }
    }
    //Power

    compx minus = {-1.0,0};


    compx vac = EE(v_compx[2],minus);
    compx Ia_conj = {i_compx[0].real,-1*(i_compx[0].imag)};
    compx Ib_conj = {i_compx[1].real,-1*(i_compx[1].imag)};
    compx S = {0,0};
    compx t3 = EE(vac,Ia_conj);
    S.real = t3.real;
    S.imag = t3.imag;
    t3 = EE(v_compx[1],Ib_conj);
    S.real += t3.real;
    S.imag += t3.imag;
    double P = S.real;
    double Q = S.imag;
    double IAP = qSqrt(S.real*S.real+S.imag*S.imag);
    double PF = P/IAP;
    double pwr_ans[4] = {P,Q,IAP,PF};
    QString pwr[4] = {"實功 :","虛功 :","視在功率 :","三項功率因數 :"};

    for(int i=0;i<4;i++){
        label_pwr[i]->setText(QString(pwr[i]).append(QString::number(pwr_ans[i])));
    }

    label9->setText("線電壓不平衡率: ");
    label9->setText(label9->text().append(QString::number(L_V_U_R*100).append(" %")));
    label10->setText("線電流不平衡率: ");
    label10->setText(label10->text().append(QString::number(L_I_U_R*100).append(" %")));

    label11->setText("Vab電壓偏差率 :");
    label11->setText(label11->text().append(QString::number(qFabs(Vrms[0]-220)*100/220)).append(" %"));
    label12->setText("Vbc電壓偏差率 :");
    label12->setText(label12->text().append(QString::number(qFabs(Vrms[1]-220)*100/220)).append(" %"));
    label13->setText("Vca電壓偏差率 :");
    label13->setText(label13->text().append(QString::number(qFabs(Vrms[2]-220)*100/220)).append(" %"));
    label14->setText("Ia電流偏差率 :");
    label14->setText(label14->text().append(QString::number(qFabs(Irms[0]-0.6)*100/0.6)).append(" %"));
    label15->setText("Ib電流偏差率 :");
    label15->setText(label15->text().append(QString::number(qFabs(Irms[1]-0.6)*100/0.6)).append(" %"));
    label16->setText("Ic電流偏差率 :");
    label16->setText(label16->text().append(QString::number(qFabs(Irms[2]-0.6)*100/0.6)).append(" %"));


    //
    if(Vrms[0]>220){
        label2->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Vrms[0]<200){
        label2->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }else{
        label2->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if(Vrms[1]>220){
        label3->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Vrms[1]<200){
        label3->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }else{
        label3->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if(Vrms[2]>220){
        label4->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Vrms[2]<200){
        label4->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }else{
        label4->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if(Irms[0]>0.6){
        label5->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Irms[0]<0.48){
        label5->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }else{
        label5->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if(Irms[1]>0.6){
        label6->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Irms[1]<0.48){
        label6->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }else{
        label6->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if(Irms[2]>0.6){
        label7->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else if(Irms[2]<0.48){
        label7->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }else{
        label7->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if(L_V_U_R>0.1){
        label9->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else{
        label9->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if(L_I_U_R>0.1){
        label10->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else{
        label10->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if((qFabs(Vrms[0]-220)*100/220)>1){
        label11->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else{
        label11->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if((qFabs(Vrms[1]-220)*100/220)>1){
        label12->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else{
        label12->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if((qFabs(Vrms[2]-220)*100/220)>1){
        label13->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else{
        label13->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if((qFabs(Irms[0]-0.6)*100/0.6)>25){
        label14->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else{
        label14->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if((qFabs(Irms[1]-0.6)*100/0.6)>25){
        label15->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else{
        label15->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if((qFabs(Irms[2]-0.6)*100/0.6)>25){
        label16->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else{
        label16->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }
    if(PF<0.6){
        label_pwr[3]->setStyleSheet("QLabel { background-color : blue; color : red; }");
    }else if(PF>0.8){
        label_pwr[3]->setStyleSheet("QLabel { background-color : red; color : blue; }");
    }else{
        label_pwr[3]->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%); color : black; }");
    }

    //
    chart->scroll(66, 0);
    chart2->scroll(66,0);

    if(!m_replyGotError)
    {
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage(QString("Got a reply"), 2000);
        ui->getButton->setEnabled(true);
    }

    m_reply->deleteLater();
}
void MainWindow::ReplyFinished2()
{
    ui->headerText->setPlainText("");
    QList<QByteArray> headerList = m_reply2->rawHeaderList();
    for(int i = 0; i < headerList.length(); ++i)
    {
        QString headerNameValuePair(headerList[i]);
        headerNameValuePair += " :       ";
        headerNameValuePair += m_reply2->rawHeader(headerList[i]);
        headerNameValuePair += "\n";

        ui->headerText->appendPlainText(headerNameValuePair);
    }
    QString source =  QString(m_reply2->readAll());

    if(!m_reply2GotError)
    {
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage(QString("Got a reply"), 2000);
        ui->getButton->setEnabled(true);
    }

    m_reply2->deleteLater();
}
void MainWindow::ReplyError(QNetworkReply::NetworkError code)
{
    m_replyGotError = true;

    QString statusMsg;
    statusMsg.sprintf("Network error: %d (%s)", (int)code, m_reply->errorString().toUtf8().constData());

    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(statusMsg);
    ui->getButton->setEnabled(true);
    //ui->postButton->setEnabled(true);
}
void MainWindow::ReplyError2(QNetworkReply::NetworkError code)
{
    m_replyGotError = true;

    QString statusMsg;
    statusMsg.sprintf("Network error: %d (%s)", (int)code, m_reply2->errorString().toUtf8().constData());

    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(statusMsg);
    ui->getButton->setEnabled(true);
    //ui->postButton->setEnabled(true);
}
void MainWindow::ReplySSLError(const QList<QSslError> & errors)
{
    m_replyGotError = true;

    QString statusMsg("SSL Network error");

    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(statusMsg);
    ui->getButton->setEnabled(true);
    //ui->postButton->setEnabled(true);
}
void MainWindow::ReplySSLError2(const QList<QSslError> & errors)
{
    m_reply2GotError = true;

    QString statusMsg("SSL Network error");

    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(statusMsg);
    ui->getButton->setEnabled(true);
    //ui->postButton->setEnabled(true);
}
void MainWindow::on_getButton_clicked()
{
    if(first_set ==false){
        get_timer = new QTimer(this);
        connect(get_timer,SIGNAL(timeout()) ,this, SLOT(MySlot()));
        post_timer = new QTimer(this);
        connect(post_timer,SIGNAL(timeout()),this,SLOT(MyPost()));
        first_set = true;
    }else{
        //get_timer->stop();
        //post_timer->stop();
    }
    if(ui->comboBox->currentText()==QString("GET")){
        ui->statusbar->showMessage(QString("Performing GET"),1000);
        doRequest(REQ_GET);
        fflush(stdout);
        qDebug () << "yes";


        get_timer->start(40000);

        ui->getButton->setEnabled(false);
    }
    else if(ui->comboBox->currentText()==QString("POST")){
        ui->statusbar->showMessage(QString("Performing POST"),1000);
        doRequest(REQ_POST);
        post_timer->start(40000);

    }

    //ui->bodyText->setPlainText(ui->comboBox->currentText());
}
void MainWindow::MySlot(){

    //qDebug () << "Myslot executed";
    ui->statusbar->showMessage(QString("Performing Data Analyze"),10000);
    doRequest(REQ_GET);

}
void MainWindow::MyPost(){
    qDebug () << "Mypost executed";
    doRequest(REQ_POST);
}
MainWindow::~MainWindow()
{
    delete ui;
}

