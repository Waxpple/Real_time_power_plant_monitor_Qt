/*
 * example.h
 *
 *  Created on: 2020年5月8日
 *      Author: Administrator
 */

#ifndef EXAMPLE_H_
#define EXAMPLE_H_
struct compx {float real,imag;};
#define  PI     3.1415926535897932384626433832795028841971               //定義圓周率值
#define  FFT_N    4096                                                    //定義福利葉變換的點數
#define  the_zero 1.0

void hellolib(const char *path);
void hellomax(int *a);
void hellofloat(float *a);
void givefloat(float *a);
void readcsv(float *a,int col);
float rms(float x[], int n);
float rms2(float x[], int n);
float avg(float x[],int n);
float max_distance(float x[],int n,float avg);
int max_index(float x[],int n,float avg);
double arctan_2pi(compx s);
struct compx EE(struct compx a,struct compx b);
void FFT(struct compx *xin);
struct compx compx_sub(struct compx a,struct compx b);
double compx_abs(struct compx a);
void get_url_2();
void FFT_length(struct compx *xin,int length);
#endif /* EXAMPLE_H_ */
