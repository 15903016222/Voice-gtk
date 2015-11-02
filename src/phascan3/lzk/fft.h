/*
 * fft.h
 *
 *  Created on: 2013-4-22
 *      Author: lzk
 */

#ifndef FFT_H_
#define FFT_H_

struct scanData_t
{
	unsigned char* data;
	int numOfData;
};

struct fftStruct
{
	double hzPerPoint;
	int peakPoint;
	int db6minPoint;
	int db6maxPoint;
	int db20minPoint;
	int db20maxPoint;
//	int fFtValid;//用于限制点数和range
	int enableFft;
};

double FftGetCenterFreq(struct fftStruct* pFft ,int db);
void fft(struct fftStruct* pFft ,struct scanData_t* in ,struct scanData_t* out);


#endif /* FFT_H_ */
