/*
 * fft.c
 *
 *  Created on: 2012-3-13
 *      Author: lzk
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "fft.h"
/*定义复数类型*/
typedef struct
{
	double real;
	double img;
}
	complex;

#define PI 3.1415926535897931
static complex* Wn = NULL; /*变换核*/
static complex* g_pData = NULL;

void fft_complex(complex* p_data ,int n);//fft算法
static void fftDataInit(int n);
int fft_get_n(int num_of_data);

double FftGetCenterFreq(struct fftStruct* pFft ,int db)
{
	double retvar = 0.0;
	if(6 == db)
	{
		retvar = pFft->hzPerPoint * pFft->peakPoint < 5 ?pFft->hzPerPoint * sqrt(pFft->db6minPoint * pFft->db6maxPoint)
			:pFft->hzPerPoint * (pFft->db6minPoint + pFft->db6maxPoint) / 2;
	}
	else if(20 == db)
	{
		retvar = pFft->hzPerPoint * pFft->peakPoint < 5 ?pFft->hzPerPoint * sqrt(pFft->db20minPoint * pFft->db20maxPoint)
			:pFft->hzPerPoint * (pFft->db20minPoint + pFft->db20maxPoint) / 2;
	}
	return retvar;
}

//fft算法应用
void fft(struct fftStruct* pFft ,struct scanData_t* in ,struct scanData_t* out)
{

	int nn,i;
	double temp;
	static int n = 0;
//	nn = fft_get_n((out->numOfData > in->numOfData) ?out->numOfData :in->numOfData);
	nn = 1024;

	if(!nn)
		return;
	if(nn != n)
	{
		n = nn;
		fftDataInit(n);
	}
	pFft->hzPerPoint = 50.0 / n;

	for(i = 0 ;i < n ;i++)
	{
		if(i < in->numOfData)
		{
			g_pData[i].real = in->data[i];
			g_pData[i].img = 0.0;
		}
		else
		{
			g_pData[i].real = 128.0;
			g_pData[i].img = 0.0;
		}
	}

	fft_complex(g_pData ,n);

	double max = 0;
	pFft->peakPoint = 0;

	out->data[0] = 0;//丢掉直流部分
	for(i = 1 ;i < out->numOfData ;++i)
	{
		if( (g_pData[i].img < 0.0001)
		&& (g_pData[i].img >-0.0001)
		)
		{
			temp = g_pData[i].real;
		}
		else
		{
			temp = sqrt(g_pData[i].real * g_pData[i].real + g_pData[i].img * g_pData[i].img);
		}
		if(temp > 0)
		{
			//temp = temp * 2 / n;//真值
			//temp =50 * log10(temp);
			temp =16 * log2(temp);
		}
		else
			temp = 0;

		if(temp > max)
		{
			max = temp;
			pFft->peakPoint = i;
		}

		out->data[i] = temp;
		if(temp > 255)
			out->data[i] = 255;
	}
	//计算衰减
	//-6db = 20lg(a/b) = 20/50(50*lg(max) - 50*lg(x)) = 20 / 16 /log2(10) * (16 * log2(max) - 16 * log2(x))
	temp = 16 * log2(10) / 20;
	//temp = 50 / 20;
	unsigned char data_db6 = out->data[pFft->peakPoint] - 6 * temp > 0 ?out->data[pFft->peakPoint] - 6 * temp :0;
	unsigned char data_db20 = out->data[pFft->peakPoint] - 20 * temp > 0 ?out->data[pFft->peakPoint] - 20 * temp :0;
	//对应的位置，初始值，也是没找到的默认值
	pFft->db6minPoint = 1;
	pFft->db6maxPoint = out->numOfData;
	pFft->db20minPoint = 1;
	pFft->db20maxPoint = out->numOfData;
	for(i = pFft->peakPoint ;i > 0 ;--i)
	{
		if(data_db6 > out->data[i])
		{
			pFft->db6minPoint = i;
			break;
		}
	}
	for(i = pFft->db6minPoint ;i > 0 ;--i)
	{
		if(data_db20 > out->data[i])
		{
			pFft->db20minPoint = i;
			break;
		}
	}
	for(i = pFft->peakPoint ;i < out->numOfData ;++i)
	{
		if(data_db6 > out->data[i])
		{
			pFft->db6maxPoint = i;
			break;
		}
	}
	for(i = pFft->db6maxPoint ;i < out->numOfData ;++i)
	{
		if(data_db20 > out->data[i])
		{
			pFft->db20maxPoint = i;
			break;
		}
	}//*/

}

int fft_get_n(int num_of_data)
{
	int i = 0;
	while(num_of_data > (int)(pow(2 ,i++)));
	return (int)(pow(2 ,i - 1));
}

static void fftDataInit(int n)
{
	int i;

	if(Wn)
	{
		free(Wn);
		Wn = NULL;
	}
	Wn = (complex *)malloc(sizeof(complex) * n);
	memset(Wn ,0 ,sizeof(complex) * n);
	for(i = 0 ;i < n ;i++)
	{
	   Wn[i].real = cos(2 * PI / n * i);
	   Wn[i].img  = -1 * sin(2 * PI / n * i);
	}

	if(g_pData)
	{
		free(g_pData);
		g_pData = NULL;
	}

	g_pData = (complex *)malloc(sizeof(complex) * n);
	memset(g_pData ,0 ,sizeof(complex) * n);

}

static void fft_add(complex a,complex b,complex *c)
{
	c->real = a.real + b.real;
	c->img = a.img + b.img;
}

static void fft_mul(complex a,complex b,complex *c)
{
	c->real = a.real * b.real - a.img * b.img;
	c->img = a.real * b.img + a.img * b.real;
}

static void fft_sub(complex a,complex b,complex *c)
{
	c->real = a.real - b.real;
	c->img = a.img - b.img;
}

static void fft_data_rearrangement(complex* p_data ,int n)
{
	complex temp;
	unsigned short i=0,j=0,k=0;
	double t ,log2n = log2(n);// / log(2);
	for(i = 0;i < n;i++)
	{
		k=i;j=0;
		t = log2n;
		while( (t--)>0 )
		{
			j <<= 1;
			j |= (k & 1);
			k >>= 1;
		}
		if(j > i)
		{
			temp 		= p_data[i];
			p_data[i] 	= p_data[j];
			p_data[j] 	= temp;
		}
	}
}

void fft_complex(complex* p_data ,int n)
{
	int i,j,k,l;
	complex up,down,product;
	double log2n = log2(n);// / log(2);
	fft_data_rearrangement(p_data ,n);
	for(i=0;i< log2n ;i++)
	{   /*一级蝶形运算*/
		l = 1 << i;
		for(j = 0 ;j < n ;j += 2 * l)
		{             /*一组蝶形运算*/
			for(k = 0 ;k < l ;k++)
			{        /*一个蝶形运算*/
				fft_mul(p_data[j + k + l] ,Wn[n * k / 2 / l] ,&product);
				fft_add(p_data[j + k] ,product ,&up);
				fft_sub(p_data[j + k] ,product ,&down);
				p_data[j + k] = up;
				p_data[j + k + l] = down;
			}
		}
	}
}

//***************************another fft: only real in***************************************

void fft_real(unsigned char* p_data_in ,complex* p_data_out ,int num_of_data)
{
	int i ,j ;
	for(i = 0 ;i < num_of_data ;++i)
	{
		p_data_out[i].real = p_data_in[0];
		p_data_out[i].img = 0;
		for(j = 1 ;j < num_of_data ;++j)
		{
			p_data_out[i].real += p_data_in[j] * cos(j * i * 2 * PI / (double)num_of_data);
			p_data_out[i].img += p_data_in[j] * sin(j * i * 2 * PI / (double)num_of_data);
		}
	}
}
