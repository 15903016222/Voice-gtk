/*
 * tofd.h
 *
 *  Created on: 2013-11-19
 *      Author: lzk
 */

#ifndef TOFD_H_
#define TOFD_H_

#define NUMOFPOINT 10

struct DefectStruct
{
	double t;//us //声程轴 时间
	int x;//pixel // 扫查轴 像素
};

struct TofdStruct
{
	int nGroup ;
	double c;//velocity (mm/us)
	double pcs;//mm
	double t0;//楔块延时 us
	double mmPerPixel;
};

double GetDepth(int grp ,double t);
double GetDepthCal(int grp, double pos, int flage);
double GetHeight(int grp , double pos1 , double pos2);
double GetLength(struct TofdStruct* pTofd ,struct DefectStruct* pDefect1 ,struct DefectStruct* pDefect2);
void GetPointer(struct TofdStruct* pTofd ,struct DefectStruct* pDefect ,double* xx ,int numOfPoint);

struct TofdStraighteningStruct
{
	double ur;
	double um;
	double sr;
	double sm;
	double averagePos;
	int intAveragePos;
	int intUr;
	int intUm;
	int intSr;
	int intSm;
	int straighteningEnable;
	int removeLateralWaveEnable;
	int dataPoints;
	float refLine;
};
#define MAX_GROUP   8
extern struct TofdStraighteningStruct gTofdS[MAX_GROUP];
void TofdGetPara(int grp);
void TofdHandler(int i ,int grp ,int width ,int _nDataOffset ,int _nDataSize);
void SetDefaultPCS(int grp);
void SetPCS(int grp ,double wedgeSeparation);

void SetDefaultTOFTCal();
int MenuHandler_TofdRefLineDepth(void* p_para);
int MenuHandler_TofdDepthCal(void* p_para);

#endif /* TOFD_H_ */
