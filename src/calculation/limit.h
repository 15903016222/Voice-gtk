/*
 * limit.h
 *
 *  Created on: 2012-10-18
 *      Author: wolflord
 */

#ifndef LIMIT_H_
#define LIMIT_H_

#include "../version.h"

#define PA_MAX_GAIN     (80)    /* PA MAX Gain */
#define UT_MAX_GAIN_1   (80)    /* FPGA1 UT MAX Gain */
#define UT_MAX_GAIN_2   (110)   /* FPGA2 UT MAX Gain */

#define DATA_SAVE_BLOCK_SIZE    1024

extern int    ConfirmGainOffsetOfAllBeamInLimit(int grp) ;
extern double GetGainOffsetLimit(int grp) ;
extern int    GetGroupDataSize(int grp) ;
extern int    GetTotalDataSize();
extern int    getDataBlockSize() ;
extern int    RefreshPointQty()  ;
extern int GetCurrentGroupLawsQtyLimit(int grp) ;
extern int isScanRangeInvalidate() ;
extern void ConfirmPulserReceiverInRange(int grp);
extern int getPulserReceiverRange(int grp) ;
extern int RefractAngleRange(double *lower, double *upper, GROUP *fgroup, int choose);
extern void ResolutionConflict(int index);
#endif /* LIMIT_H_ */
