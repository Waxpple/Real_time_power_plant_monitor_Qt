/*
 * example.cpp
 *
 *  Created on: 2020撟�5���8�
 *      Author: Administrator
 */


#include <iostream>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include "example.h"
#include "restclient-cpp/restclient.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
using namespace rapidjson;
using namespace std;

float rms(float x[], int n) {
	double sum = 0;

	for (int i = 0; i < n; i++)
		sum += pow(x[i], 2);

	return sqrt(sum / n);
}
float rms2(float x[], int n){
	double sum = 0;

		for (int i = 0; i < n; i++)
			sum += pow(x[i], 2);

		return sqrt(sum);
}

float avg(float x[],int n){
	double sum =0;
	for (int i =0;i <n;i++)
		sum += x[i];

	return sum/n;
}
float max_distance(float x[],int n,float avg){
	float difference=0;
	float max_value;

	for(int i=0;i<n;i++){

		if(fabs(fabs(x[i])-avg)>difference){
			difference = fabs(fabs(x[i])-avg);
			max_value = x[i];
		}

	}
	return difference;

}
int max_index(float x[],int n,float avg){
	float difference=0;
	int max_index=-1;

	for(int i=0;i<n;i++){

		if(fabs(fabs(x[i])-avg)>difference){
			difference = fabs(fabs(x[i])-avg);
			max_index = i;
		}

	}
	return max_index;

}
class CSVRow {
public:
	std::string const& operator[](std::size_t index) const {
		return m_data[index];
	}
	std::size_t size() const {
		return m_data.size();
	}
	void readNextRow(std::istream &str) {
		std::string line;
		std::getline(str, line);

		std::stringstream lineStream(line);
		std::string cell;

		m_data.clear();
		while (std::getline(lineStream, cell, ',')) {
			m_data.push_back(cell);
		}
		// This checks for a trailing comma with no data after it.
		if (!lineStream && cell.empty()) {
			// If there was a trailing comma then add an empty element.
			m_data.push_back("");
		}
	}
private:
	std::vector<std::string> m_data;
};

std::istream& operator>>(std::istream &str, CSVRow &data) {
	data.readNextRow(str);
	return str;
}
void hellolib(const char *path){
	printf("path=%s\n",path);
}

void helloint(int *a){
	for(int i=0;i<2;i++){
		printf("the ans is %d\n",*(a+1*i));
	}

}
void hellofloat(float *a){
	for(int i=0;i<100;i++){
		printf("the ans is %f\n",*(a+1*i));
	}

}
void givefloat(float *a){
	for(int i=0;i<2;i++){
		*(a+1*i) = (float)i;
	}

}
void readcsv(float *a,int col){
	ifstream file("C:\\Users\\Administrator\\Downloads\\data123.csv");

	size_t length = count(istreambuf_iterator<char>(file),
			istreambuf_iterator<char>(), '\n');
	printf("lines : %d\n", length);
	file.clear();
		file.seekg(0);

		//float data[6][length];
		size_t c = 0;



		CSVRow row;
		while (file >> row) {
			if(col<3){
				*(a+c) = stof(row[col])*80;
			}else{
				*(a+c) = stof(row[col]);
			}
//			data[1][c] = stof(row[1]);
//			data[2][c] = stof(row[2]);
//			data[3][c] = stof(row[3]);
//			data[4][c] = stof(row[4]);
//			data[5][c] = stof(row[5]);
			c++;
		}
}
struct compx EE(struct compx a,struct compx b)
{
	struct compx c;

	c.real=a.real*b.real-a.imag*b.imag;
	c.imag=a.real*b.imag+a.imag*b.real;

	return(c);
}
struct compx compx_sub(struct compx a,struct compx b)
{
	struct compx c;
	c.real=a.real-b.real;
	c.imag=a.imag-b.imag;

	return(c);

}
double compx_abs(struct compx a)
{
	double c;

	c = sqrt(a.real*a.real+a.imag*a.imag);

	return(c);
}
/***
 *
 */
double arctan_2pi(compx s){
	double result = atan((double)s.imag/s.real) * 180 / PI;
	if(s.real<0 and s.imag >=0){
				result = result+180;
	}else if(s.real<0 and s.imag <0){
				result = result -180;
	}
	if (result<0){
		result = result+360;
	}
	return result;

}
void FFT(struct compx *xin)
{
	int f , m, nv2, nm1, i, k, l, j = 0;
	struct compx u,w,t;

	nv2 = FFT_N / 2;                   //���嚙踝蕭嚙踝�蕭謅�嚙踝�嚙踝蕭��嚙踝嚙踝蕭��蕭�嚙踝�蕭��蕭��蕭��蕭�嚙踝嚙踝��蕭謆��蕭謅蕭嚙�
	nm1 = FFT_N - 1;
	for(i = 0; i < nm1; i++)
	{
		if(i < j)                      //���蕭謓�<j,嚙踝嚙質嚙踝嚙踝�嚙�
		{
			t = xin[j];
			xin[j] = xin[i];
			xin[i] = t;
		}
		k = nv2;                       //��蝙嚙踝蕭��蕭��蕭嚙踝�蕭��蕭��蕭嚙�
		while( k <= j)                 //���蕭謓�<=j,���j嚙踝蕭��蕭��朴嚙踝�冪1
		{
			j = j - k;                 //嚙踝蕭��蕭��朴嚙踝�蕭��蕭嚙�0
			k = k / 2;                 //k/2��嚙踐�蕭�瘣餅�朴嚙踝嚙踝嚙踐�暑�豯伍��嚙踐�蕭��蕭��蕭�嚙踝謆蕭��蕭嚙踐�蕭��蕭��冪0
		}
		j = j + k;                     //嚙踝蕭嚙�0嚙踐�蕭蹌�1
	}

	{
		int le , lei, ip;                            //FFT嚙踝蕭��蕭謅僚����蕭��蕭豰嚚蕭��蕭謅蕭�嚙踐�T嚙踝蕭��蕭嚙�

		f = FFT_N;
		for(l = 1; (f=f/2)!=1; l++)                  //���蕭謑蕭嚙踝�蕭瞏蕭�����蕭謅�嚗對蕭謍梱
			;
		for( m = 1; m <= l; m++)                         // 嚙踐�蕭��蕭豰嚗對蕭��蕭謍梱
		{                                        //m�������奕豰嚚蕭�嚙踐冪嚙踐�嚗對蕭謍脰�蕭�l=log��蕭2��
			le = 2 << (m - 1);                            //le嚙踐�嚗對蕭��蕭豲�������奕豰嚗對蕭���嚗對蕭����掠e�嚙�
			lei = le / 2;                               //嚙踝蕭�嚙踝蕭豰嚗對蕭���蕭嚙踝嚙踐滯嚙踝�蕭謅蕭���豯佗蕭��蕭豲��
			u.real = 1.0;                             //u嚙踐冪嚙踐�嚗對蕭��蕭��蕭謅�蕭���嚙踐�蕭��蕭瞏冪1
			u.imag = 0.0;
			w.real = cos(PI / lei);                     //w嚙踐冪���嚙踝蕭��蕭��嚙踐嚙踝蕭���蕭�嚙踝蕭�嚙踝�蕭嚙踝��蕭�嚙踝蕭��蕭嚙�
			w.imag = -sin(PI / lei);
			for(j = 0;j <= lei - 1; j++)                   //嚙踐�蕭����蕭謅蕭��蕭�頠蕭豰嚗對蕭��蕭�����蕭謅�蕭����蕭�嚙踝��嚗對蕭嚙�
			{
				for(i = j; i <= FFT_N - 1; i = i + le)            //嚙踐�蕭��蕭嚙踝嚙踝蕭豰嚗對蕭��蕭��蕭謅�嚙踝����蕭謅�蕭�嚙踐�蕭嚙踝豰嚗對蕭嚙�
				{
					ip = i + lei;                           //i���嚙踝蕭�����嚙踝蕭�嚙踐滿豰嚚蕭��蕭謅蕭��嚙踝�蕭�嚙�
					t = EE(xin[ip], u);                    //嚙踐�嚚蕭��蕭謅�嚙踝摨����蕭
					xin[ip].real = xin[i].real - t.real;
					xin[ip].imag = xin[i].imag - t.imag;
					xin[i].real = xin[i].real + t.real;
					xin[i].imag = xin[i].imag + t.imag;
				}
				u = EE(u, w);                           //嚙踐����蕭���嚙質嚙踝嚙踝�蕭嚙踝��嚚蕭��蕭嚙�
			}
		}
	}
}
void FFT_length(struct compx *xin,int length)
{
	int f , m, nv2, nm1, i, k, l, j = 0;
	struct compx u,w,t;

	nv2 = length / 2;                   //���嚙踝蕭嚙踝�蕭謅�嚙踝�嚙踝蕭��嚙踝嚙踝蕭��蕭�嚙踝�蕭��蕭��蕭��蕭�嚙踝嚙踝��蕭謆��蕭謅蕭嚙�
	nm1 = length - 1;
	for(i = 0; i < nm1; i++)
	{
		if(i < j)                      //���蕭謓�<j,嚙踝嚙質嚙踝嚙踝�嚙�
		{
			t = xin[j];
			xin[j] = xin[i];
			xin[i] = t;
		}
		k = nv2;                       //��蝙嚙踝蕭��蕭��蕭嚙踝�蕭��蕭��蕭嚙�
		while( k <= j)                 //���蕭謓�<=j,���j嚙踝蕭��蕭��朴嚙踝�冪1
		{
			j = j - k;                 //嚙踝蕭��蕭��朴嚙踝�蕭��蕭嚙�0
			k = k / 2;                 //k/2��嚙踐�蕭�瘣餅�朴嚙踝嚙踝嚙踐�暑�豯伍��嚙踐�蕭��蕭��蕭�嚙踝謆蕭��蕭嚙踐�蕭��蕭��冪0
		}
		j = j + k;                     //嚙踝蕭嚙�0嚙踐�蕭蹌�1
	}

	{
		int le , lei, ip;                            //FFT嚙踝蕭��蕭謅僚����蕭��蕭豰嚚蕭��蕭謅蕭�嚙踐�T嚙踝蕭��蕭嚙�

		f = length;
		for(l = 1; (f=f/2)!=1; l++)                  //���蕭謑蕭嚙踝�蕭瞏蕭�����蕭謅�嚗對蕭謍梱
			;
		for( m = 1; m <= l; m++)                         // 嚙踐�蕭��蕭豰嚗對蕭��蕭謍梱
		{                                        //m�������奕豰嚚蕭�嚙踐冪嚙踐�嚗對蕭謍脰�蕭�l=log��蕭2��
			le = 2 << (m - 1);                            //le嚙踐�嚗對蕭��蕭豲�������奕豰嚗對蕭���嚗對蕭����掠e�嚙�
			lei = le / 2;                               //嚙踝蕭�嚙踝蕭豰嚗對蕭���蕭嚙踝嚙踐滯嚙踝�蕭謅蕭���豯佗蕭��蕭豲��
			u.real = 1.0;                             //u嚙踐冪嚙踐�嚗對蕭��蕭��蕭謅�蕭���嚙踐�蕭��蕭瞏冪1
			u.imag = 0.0;
			w.real = cos(PI / lei);                     //w嚙踐冪���嚙踝蕭��蕭��嚙踐嚙踝蕭���蕭�嚙踝蕭�嚙踝�蕭嚙踝��蕭�嚙踝蕭��蕭嚙�
			w.imag = -sin(PI / lei);
			for(j = 0;j <= lei - 1; j++)                   //嚙踐�蕭����蕭謅蕭��蕭�頠蕭豰嚗對蕭��蕭�����蕭謅�蕭����蕭�嚙踝��嚗對蕭嚙�
			{
				for(i = j; i <= length - 1; i = i + le)            //嚙踐�蕭��蕭嚙踝嚙踝蕭豰嚗對蕭��蕭��蕭謅�嚙踝����蕭謅�蕭�嚙踐�蕭嚙踝豰嚗對蕭嚙�
				{
					ip = i + lei;                           //i���嚙踝蕭�����嚙踝蕭�嚙踐滿豰嚚蕭��蕭謅蕭��嚙踝�蕭�嚙�
					t = EE(xin[ip], u);                    //嚙踐�嚚蕭��蕭謅�嚙踝摨����蕭
					xin[ip].real = xin[i].real - t.real;
					xin[ip].imag = xin[i].imag - t.imag;
					xin[i].real = xin[i].real + t.real;
					xin[i].imag = xin[i].imag + t.imag;
				}
				u = EE(u, w);                           //嚙踐����蕭���嚙質嚙踝嚙踝�蕭嚙踝��嚚蕭��蕭嚙�
			}
		}
	}
}
void get_url_2(){
    printf("test\n");

    vector<int> USER_VAL = {4,50,0,52,14,15,17,21,41};

    rapidjson::Document document;
    document.SetArray();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    for (int i = 0; i < USER_VAL.size(); i++)
    {
        document.PushBack(USER_VAL.at(i),allocator);
    }
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    document.Accept(writer);

    printf("data is %s",strbuf.GetString());





		//std::cout << buffer.GetString() << std::endl;

//		std::vector<int> USER_VAL = {4,50,0,52,14,15,17,21,41};
//
//		rapidjson::Document document;
//		document.SetArray();
//
//		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
//		for (int i = 0; i < USER_VAL.size(); i++)
//		{
//		    document.PushBack(USER_VAL.at(i),allocator);
//		}
//		rapidjson::StringBuffer strbuf;
//		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
//		document.Accept(writer);
//		printf("data %s",strbuf.GetString());

}
