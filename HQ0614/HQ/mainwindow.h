#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtCore>
#include <QtCharts>
#include <cstring>
#include <QtCharts/QPolarChart>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <example.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void doRequest(int reqType);
    enum
    {
        REQ_GET,
        REQ_POST
    };
    QTimer *get_timer;
    QTimer *post_timer;

public slots:
    void removeSeries(QList<QSplineSeries *> m_series, QChart* chart);
    void connectMarkers(QChart* chart);
    void disconnectMarkers();
    void handleMarkerClicked();
    void on_getButton_clicked();
    void ReplyFinished();
    void ReplyFinished2();
    void ReplyError(QNetworkReply::NetworkError code);
    void ReplySSLError(const QList<QSslError> & errors);
    void ReplyError2(QNetworkReply::NetworkError code);
    void ReplySSLError2(const QList<QSslError> & errors);
    void MySlot();
    void MyPost();
private slots:



private:
    QPolarChart *polar_chart;

    QNetworkAccessManager *m_netManager;
    QNetworkAccessManager *m_netManager_2;
    QNetworkReply *m_reply;
    QNetworkReply *m_reply2;
    bool m_replyGotError;
    bool m_reply2GotError;
    bool first_set;
    QSplineSeries *series[6];
    QList<QSplineSeries *> m_series;
    //dynamic
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;
    QLabel *label6;
    QLabel *label7;
    QLabel *label9;
    QLabel *label10;
    QLabel *label11;
    QLabel *label12;
    QLabel *label13;
    QLabel *label14;
    QLabel *label15;
    QLabel *label16;
    QLabel *label17;
    QLabel *label18;
    QLabel *label_thd[6];
    QChart *chart;
    QChartView *chartView;
    int now_loc;
    rapidjson::Document document;
    QJsonObject json;
    unsigned int length;
    float data[6][4096];
    compx data_fft[6][4096];
    float abs_data_fft[6][4096];
    compx *fft_p[6];
    float *p[6];
    QSplineSeries *polar_series1;
    QSplineSeries *polar_series2;
    QSplineSeries *polar_series3;
    QSplineSeries *polar_series4;
    QSplineSeries *polar_series5;
    QSplineSeries *polar_series6;
    QSplineSeries *series_fft[6];
    float Vrms[3],Irms[3];
    float L_V_U_R,L_I_U_R;
    float rms_avg[2]={0.0,0.0};
    double polar_angular[6];
    double polar_radian[6];
    double polar_cail_angular[6];
    int freq_index;
    compx v_compx[3],i_compx[3];
    float VUF;
    float IUF;
    double thd_ans[6];
    int counter;
    QLabel *label_pwr[4];
    //
    //QList<QSplineSeries *> m_series2;
    QChart *chart2;
    QChart *chart3,*chart4;
    QLabel *now_index;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
