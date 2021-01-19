#include "http.h"
#include<QCoreApplication>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include<QJsonArray>
#include<QDebug>
#include<QString>
http::http(QString strr)
{
        QString json="{"
                                "\"Name\":\"God\","
                                "\"Data\":[\"Loli\",5,3.14,false]"
                            "}";
        json = strr;
       QJsonDocument d=QJsonDocument::fromJson(json.toUtf8());
       QJsonObject json_obj=d.object();

       //輸出：
       QString strJson=d.toJson();
       qDebug()<<strJson.toStdString().data();
       qDebug()<<"---------------------------------";

       //輸出：
       QString name=json_obj["response"].toString();
       qDebug()<<"YourNameIs:"<<name.toStdString().data();

       //輸出：
       QJsonArray json_data=json_obj["Data"].toArray();
       for(int i=0; i<json_data.size(); i++){
           if(json_data[i].isString()){
               qDebug()<<"Data"<<i<<":"<<json_data[i].toString().toStdString().data();
           }else if(json_data[i].isDouble()){
               qDebug()<<"Data"<<i<<":"<<json_data[i].toDouble();
           }else if(json_data[i].isBool()){
               qDebug()<<"Data"<<i<<":"<<json_data[i].toBool();
           }
       }
}
