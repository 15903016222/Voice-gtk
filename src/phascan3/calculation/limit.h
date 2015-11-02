/*
 * limit.h
 *
 *  Created on: 2012-10-18
 *      Author: wolflord
 */

#ifndef LIMIT_H_
#define LIMIT_H_

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
